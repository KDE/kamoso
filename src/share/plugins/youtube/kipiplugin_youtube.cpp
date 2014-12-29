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

#include "kipiplugin_youtube.h"
#include "youtubejobcomposite.h"
#include "youtubejob.h"

#include <QAction>
#include <QApplication>
#include <QDesktopServices>
#include <QIcon>

#include <KPluginFactory>
#include <KAboutData>
#include <QMimeType>
#include <QUrl>
#include <kpassworddialog.h>
#include <kwallet.h>
#include <QDebug>

K_PLUGIN_FACTORY_WITH_JSON(YoutubeShare, "youtubeplugin.json", registerPlugin<YoutubePlugin>();)

EXPORT_SHARE_VERSION

YoutubePlugin::YoutubePlugin(QObject* parent, const QVariantList& args)
    : SharePlugin(parent)
{
    Q_UNUSED(args);
}

ShareJob* YoutubePlugin::share() const
{
    return new YoutubeJobComposite;
}

#include "kipiplugin_youtube.moc"
