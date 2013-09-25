/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <afiestas@kde.org>                       *
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

#ifndef KIPIPLUGIN_YOUTUBE_H
#define KIPIPLUGIN_YOUTUBE_H

#include <KPasswordDialog>
#include <KUrl>
#include <kwallet.h>
#include <KIO/Job>
#include <libkipi/plugin.h>
#include <exportinterface.h>

class YoutubePlugin : public KIPI::Plugin, public ExportInterface
{
	Q_OBJECT
	public:
		YoutubePlugin(QObject* parent, const QVariantList& args);
// 		virtual QAction* thumbnailsAction(const QList<KUrl>& url);
// 		void login();
		
	virtual KIPI::Category category(KAction* action) const;
	virtual void setup(QWidget* widget);
	
    virtual KJob* exportFiles(const QString& albumname);
	
	public slots:
// 		void upload();
// 		void authenticated(bool);
// 		void loginDone(KIO::Job *job, const QByteArray &data);
	private:
// 		QList<KUrl> mSelectedUrls;
// 		KWallet::Wallet *m_wallet;
// 		QString videoTitle;
// 		QString videoDesc;
// 		QString videoTags;
// 		QByteArray m_authToken;
// 		KPasswordDialog *dialog;
};

#endif /* KIPIPLUGIN_YOUTUBE_H */
