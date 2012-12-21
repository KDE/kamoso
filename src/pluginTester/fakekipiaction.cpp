/*************************************************************************************
 *  Copyright (C) 2008-2010 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2010 by Alex Fiestas <alex@eyeos.org>                         *
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

#include "fakekipiaction.h"

#include <libkipi/plugin.h>
#include <KIO/JobUiDelegate>
#include <kjobtrackerinterface.h>
#include <KDebug>
#include "../plugins/exportinterface.h"

FakeKipiAction::FakeKipiAction(KIPI::PluginLoader::Info* pluginInfo, QObject* parent)
    : QAction(pluginInfo->icon(), pluginInfo->name(), parent), pluginInfo(pluginInfo)
{
    connect(this, SIGNAL(triggered()), SLOT(runJob()));
}

FakeKipiAction::~FakeKipiAction()
{
    KIO::getJobTracker()->unregisterJob(m_job);
}

void FakeKipiAction::runJob()
{
    KIPI::Plugin* p=pluginInfo->plugin();
    ExportInterface* ep = dynamic_cast<ExportInterface*>(p);

    m_job = ep->exportFiles(i18n("Kamoso"));
    KIO::getJobTracker()->registerJob(m_job);
    m_job->start();
    
    connect(m_job, SIGNAL(finished(KJob*)), SLOT(done(KJob*)));
}

void FakeKipiAction::done(KJob* job)
{
    kDebug() << "Finished job: " << job->objectName();
    if(job->error()!=0)
        kDebug() << "the job finished with error" << job->error() << job->errorString();
}
