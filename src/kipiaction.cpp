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

#include "kipiaction.h"
#include <libkipi/plugin.h>
#include "plugins/exportinterface.h"
#include <QDebug>
#include "kamoso.h"
#include "kamosojobtracker.h"

KipiAction::KipiAction(KIPI::PluginLoader::Info* pluginInfo, Kamoso* ui, QObject* parent)
    : QAction(pluginInfo->icon(), pluginInfo->name(), parent), pluginInfo(pluginInfo), mKamoso(ui)
{
    connect(this, SIGNAL(triggered()), SLOT(runJob()));
}

void KipiAction::runJob()
{
    KIPI::Plugin* p=pluginInfo->plugin();
    ExportInterface* ep=dynamic_cast<ExportInterface*>(p);

    KJob* job=ep->exportFiles(i18n("Kamoso"));
    mKamoso->tracker()->registerJob(job, mKamoso->selectedItems(), icon());
}
