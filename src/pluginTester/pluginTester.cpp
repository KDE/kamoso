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

#include "pluginTester.h"
#include "fakekipiinterface.h"
#include "fakekipiaction.h"

#include <QDebug>
#include <kurl.h>
#include <KCmdLineArgs>
#include <libkipi/plugin.h>
#include <libkipi/pluginloader.h>

PluginTester::PluginTester(QObject *parent) : QObject(parent)
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    QString pluginName = args->arg(0);

    qDebug() << "PluginName: " << pluginName;
    qDebug() << "Files: ";
    KUrl::List kurlList;
    for (int i = 1; i < args->count(); ++i) {
        qDebug() << "\t" << args->arg(i);
        kurlList.append(KUrl(args->arg(i)));
    }

    m_pluginLoader = new KIPI::PluginLoader(QStringList(), new FakeKIPIInterface(kurlList), "");

    Q_FOREACH(KIPI::PluginLoader::Info *pluginInfo, m_pluginLoader->pluginList()) {
        if (pluginInfo->service()->name() == "YouTube") {
            m_action = new FakeKipiAction(pluginInfo, this);
            m_action->trigger();
        }
    }
}

PluginTester::~PluginTester()
{
    delete m_action;
}