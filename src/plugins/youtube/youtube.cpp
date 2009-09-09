/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@gmail.com>                        *
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

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <kwallet.h>
#include "youtube.h"
// #include "youtubemanager.h"
#include "youtubejob.h"
#include "src/plugins/youtube/ui_auth.h"
#include "src/plugins/youtube/ui_videoInfo.h"

using KWallet::Wallet;

K_PLUGIN_FACTORY(KamosoYoutubeFactory, registerPlugin<YoutubePlugin>(); )
K_EXPORT_PLUGIN(KamosoYoutubeFactory(KAboutData("youtube", "youtube",
		ki18n("Youtube"), "0.1", ki18n("Runs a visor for the specified file."),
		KAboutData::License_GPL)))

YoutubePlugin::YoutubePlugin(QObject* parent, const QVariantList& args)
	: KamosoPlugin(parent, args)
{
	m_auth = new Ui::authWidget;
	m_authWidget = new QWidget();
	m_auth->setupUi(m_authWidget);
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
				act=new QAction(KIcon("system-run"), i18n("Upload to youtube"), 0);
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
	if(!m_wallet->hasFolder("youtubeKamoso")){
		if(!m_wallet->createFolder("youtubeKamoso")){
			//TODO: Error reporting here
			return;
		}
	}
	m_wallet->setFolder("youtubeKamoso");
// 	if(!m_wallet->hasEntry("youtubeAuth")){
// 		if(!showDialog()){
// 			return;
// 		}
// 	}
	//Until we've config dialog for plugins, this is the best I can do
	if(!askNewData()){
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
	dialog->setButtons(KDialog::Ok | KDialog::Cancel);
	dialog->setMinimumWidth(425);
	dialog->setMinimumHeight(315);
	dialog->setMaximumWidth(425);
	dialog->setMaximumHeight(315);
	int response = dialog->exec();
	
	QMap<QString, QString> videoInfo;
	if(response == QDialog::Accepted){
		if(videoForm->descriptionText->toPlainText().size() > 0){
			videoInfo["videoDesc"] = videoForm->descriptionText->toPlainText();
		}
		if(videoForm->titleText->text().size() > 0){
			videoInfo["videoTitle"] = videoForm->titleText->text();
		}
		if(videoForm->tagText->text().size() > 0){
			videoInfo["videoTags"] = videoForm->tagText->text();
		}
	}
	return videoInfo;
}
void YoutubePlugin::login()
{
	#warning where the hell we've to put the developerKey? in a define?
	QMap<QString, QString> authInfo;
	m_wallet->readMap("youtubeAuth",authInfo);

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
	KDialog *dialog = new KDialog();

	dialog->setMainWidget(m_authWidget);
	dialog->setCaption(i18n("Youtube authentification:"));
	dialog->setButtons(KDialog::Ok | KDialog::Cancel);
	dialog->setMinimumWidth(300);
	dialog->setMaximumWidth(300);
	dialog->setMinimumHeight(110);
	dialog->setMaximumHeight(110);
	int response = dialog->exec();
	if(response == QDialog::Rejected){
		return false;
	}
	while((m_auth->usernameText->displayText() == "" || m_auth->passwordText->displayText() == "") && response == QDialog::Accepted )
	{
		response = dialog->exec();
		if(response == QDialog::Rejected){
			return false;
		}
	}
	QMap<QString, QString> toSave;
	toSave["username"] = m_auth->usernameText->text();
	toSave["password"] = m_auth->passwordText->text();
	m_wallet->writeMap("youtubeAuth",toSave);
	m_wallet->sync();
	return true;
}

void YoutubePlugin::authenticated(bool auth)
{
	qDebug() << "Authentification: " << auth ;
	if(auth == false){
		if(askNewData()){
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

bool YoutubePlugin::askNewData()
{
	QMap<QString, QString> authInfo;
	m_wallet->readMap("youtubeAuth",authInfo);
	
	m_auth->usernameText->setText(authInfo["username"]);
	m_auth->passwordText->setText(authInfo["password"]);
	return showDialog();
}