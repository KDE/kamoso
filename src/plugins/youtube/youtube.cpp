/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2009 by Alex Fiestas <alex@eyeos.org>                         *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include <KPluginFactory>
#include <KAboutData>
#include <KMimeType>
#include <KIcon>
#include <KUrl>
#include <KDialog>
#include <KMessageBox>
#include <kpassworddialog.h>

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <kwallet.h>
#include "youtube.h"
// #include "youtubemanager.h"
#include "youtubejob.h"
#include "src/plugins/youtube/ui_videoInfo.h"

using KWallet::Wallet;

K_PLUGIN_FACTORY(KamosoYoutubeFactory, registerPlugin<YoutubePlugin>(); )
K_EXPORT_PLUGIN(KamosoYoutubeFactory(KAboutData("youtube", "youtube",
		ki18n("YouTube"), "0.1", ki18n("Uploads files to YouTube"),
		KAboutData::License_GPL)))

YoutubePlugin::YoutubePlugin(QObject* parent, const QVariantList& args)
	: KamosoPlugin(parent, args), dialog(0)
{
	KIconLoader::global()->addAppDir("kamoso_youtube");
}

QAction* YoutubePlugin::thumbnailsAction(const QList<KUrl>& urls)
{
	QAction* act=0;
	mSelectedUrls.clear();
	foreach(const KUrl& url, urls)
	{
		KMimeType::Ptr mime = KMimeType::findByUrl(url);
		if(mime->name().startsWith("video/")) {
			if(!act) {
				act=new QAction(KIcon("youtube"), i18n("Upload to YouTube"), 0);
				connect(act, SIGNAL(triggered(bool)), SLOT(upload()));
			}
		}
		mSelectedUrls.append(url);
	}
	return act;
}

void YoutubePlugin::upload()
{
 	m_wallet = Wallet::openWallet(Wallet::NetworkWallet(),qApp->activeWindow()->winId());
	if(m_wallet != NULL){
		if(!m_wallet->hasFolder("youtubeKamoso")){
			if(!m_wallet->createFolder("youtubeKamoso")){
				//TODO: Error reporting here
				return;
			}
		}
		m_wallet->setFolder("youtubeKamoso");
	}

	if(!showDialog()){
		return;
	}
	login();
}

QMap<QString, QString> YoutubePlugin::showVideoDialog()
{
	Ui::videoForm *videoForm = new Ui::videoForm;
	QWidget *videoWidget = new QWidget();
	videoForm->setupUi(videoWidget);

	KDialog *dialog = new KDialog();
	dialog->setMainWidget(videoWidget);
	dialog->setCaption(i18n("Video information:"));
	dialog->setButtons(KDialog::Ok);
	dialog->setMinimumWidth(425);
	dialog->setMinimumHeight(315);
	dialog->setMaximumWidth(425);
	dialog->setMaximumHeight(315);
	int response = dialog->exec();
	
	QMap<QString, QString> videoInfo;
	if(response == QDialog::Accepted){
		if(!videoForm->descriptionText->toPlainText().isEmpty()){
			videoInfo["videoDesc"] = videoForm->descriptionText->toPlainText();
		}
		if(!videoForm->titleText->text().isEmpty()){
			videoInfo["videoTitle"] = videoForm->titleText->text();
		}
		if(!videoForm->tagText->text().isEmpty()){
			videoInfo["videoTags"] = videoForm->tagText->text();
		}
	}
	return videoInfo;
}
void YoutubePlugin::login()
{
	QMap<QString, QString> authInfo;
	authInfo["username"] = dialog->username();
	authInfo["password"] = dialog->password();

	KUrl url("https://www.google.com/youtube/accounts/ClientLogin");
	QByteArray data("Email=");
	data.append(authInfo["username"].toAscii());
	data.append("&Passwd=");
	data.append(authInfo["password"].toAscii());
	data.append("&service=youtube&source=Kamoso");
	KIO::TransferJob *loginJob = KIO::http_post(url,data,KIO::HideProgressInfo);
	loginJob->addMetaData("cookies","none");
	loginJob->addMetaData("content-type","Content-Type:application/x-www-form-urlencoded");
	connect(loginJob,SIGNAL(data(KIO::Job *, const QByteArray &)),this,SLOT(loginDone(KIO::Job *, const QByteArray &)));
	loginJob->start();
}

void YoutubePlugin::loginDone(KIO::Job *job, const QByteArray &data)
{
	delete job;
	qDebug() << "LoginDone, data received\n";
	qDebug() << data.data();
	if(data.at(0) == 'E'){
		authenticated(false);
	}else{
		QList<QByteArray> tokens = data.split('\n');
		m_authToken = tokens.first().remove(0,5);
		qDebug() << "Final AuthToken: " << m_authToken.data();
		authenticated(true);
	}
}

bool YoutubePlugin::showDialog()
{
	QString server = QString("http://www.youtube.com");
	
	if(m_wallet != NULL) {
		dialog = new KPasswordDialog(0L,KPasswordDialog::ShowKeepPassword | KPasswordDialog::ShowUsernameLine);
		QMap<QString, QString> authInfo;
		m_wallet->readMap("youtubeAuth",authInfo);
		dialog->setPassword(authInfo["password"]);
		dialog->setUsername(authInfo["username"]);
		dialog->setKeepPassword(true);
	}else{
		dialog = new KPasswordDialog(0L,KPasswordDialog::ShowUsernameLine);
	}
	dialog->setPrompt(i18n("You need to supply a username and a password to be able to upload videos to YouTube"));
	dialog->addCommentLine(i18n("Server")+": ",server);
	dialog->setCaption(i18n("Authentication for ")+"youtube");

	int response = dialog->exec();
	if(response == QDialog::Rejected){
		return false;
	}
	while((dialog->username() == "" || dialog->password() == "") && response == QDialog::Accepted )
	{
		response = dialog->exec();
		if(response == QDialog::Rejected){
			return false;
		}
	}

	if(dialog->keepPassword() == true &&  m_wallet != NULL) {
		QMap<QString, QString> toSave;
		toSave["username"] = dialog->username();
		toSave["password"] = dialog->password();
		m_wallet->writeMap("youtubeAuth",toSave);
		m_wallet->sync();
	}
	return true;
}

void YoutubePlugin::authenticated(bool auth)
{
	qDebug() << "Authentification: " << auth ;
	if(auth == false){
		if(showDialog()){
			login();
		}
		return;
	}
	QMap<QString, QString> videoInfo;
	foreach(const KUrl& path, mSelectedUrls) {
		videoInfo = showVideoDialog();
		YoutubeJob* job=new YoutubeJob(path,m_authToken,videoInfo);
		emit jobCreated(job);
	}
}
