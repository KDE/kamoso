/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
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

#include "kamosoplugin.h"
#include <QAction>

KamosoPlugin::KamosoPlugin(QObject* parent, const QVariantList& args)
	: QObject(parent)
{
    Q_UNUSED(args); //We do not need that (yet?)
}

KamosoPlugin::~KamosoPlugin()
{}

bool KamosoPlugin::executeContextMenuAction(const QList< KUrl >& urls)
{
    QAction* a=thumbnailsAction(urls);
    if(a)
        a->trigger();
    bool ret=(a!=0);
    delete a;
    return ret;
}

#include "kamosoplugin.moc"
