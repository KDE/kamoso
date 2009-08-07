/*************************************************************************************
 *  Copyright (C) 2009 by Aleix Pol <aleixpol@kde.org>                               *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 3                   *
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

#include "kamosoplugin.h"
#include <KUrl>
#include "fbtalker.h"

namespace KIPIFacebookPlugin { class FbAlbum; }

class FacebookPlugin : public KamosoPlugin
{
	Q_OBJECT
	Q_INTERFACES(KamosoPlugin)
	public:
		FacebookPlugin(QObject* parent, const QVariantList& args);
		virtual QAction* thumbnailsAction(const KUrl& url);
		
	public slots:
		void uploadImage(bool);
		void albumList(int, const QString&, const QList< KIPIFacebookPlugin::FbAlbum >&);
		void loginDone(int,QString);
		void albumCreated(int, const QString&, long long);
		
	private:
		KIPIFacebookPlugin::FbTalker talk;
		KUrl mSelectedUrls;
		void sendPhoto(long long album);
};
