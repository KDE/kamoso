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

#ifndef KAMOSOPLUGIN_H
#define KAMOSOPLUGIN_H

#include <QObject>
#include <QVariantList>
#include "kdemacros.h"

class QAction;
class KJob;
class KUrl;
class KamosoJob;

class KDE_EXPORT KamosoPlugin : public QObject
{
	Q_OBJECT
	public:
		KamosoPlugin(QObject* parent, const QVariantList& args);
		virtual ~KamosoPlugin();
		
		/** Action that it will appear in the thumbnails view's menu.
			@p url Describes the item we need it for.
			
			@returns the action to be added. If a null action is returned,
			nothing will be added
		*/
		virtual QAction* thumbnailsAction(const QList<KUrl>& url)=0;
	
	signals:
		void jobCreated(KamosoJob* job);
};
Q_DECLARE_INTERFACE(KamosoPlugin, "org.kamoso.plugin");

#endif // KAMOSOPLUGIN_H
