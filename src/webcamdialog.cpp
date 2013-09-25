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

#include "webcamdialog.h"

#include <KDebug>
#include <KLocale>

WebcamDialog::WebcamDialog( QWidget *parent, const QString& name, KConfigSkeleton *config ): 
                KConfigDialog(parent,name,config),configManager(0)
{

}
WebcamDialog::~WebcamDialog()
{
    delete configManager;
}
bool WebcamDialog::hasChanged()
{
    Q_ASSERT(configManager);
    if(KConfigDialog::hasChanged() == true || configManager->hasChanged() == true) {
        return true;
    }
    return false;
}
void WebcamDialog::setPageWebcamConfigManager(PageWebcamConfigManager* webcamManager)
{
    configManager = webcamManager;
}
void WebcamDialog::updateSettings()
{
    Q_ASSERT(configManager);
    if(hasChanged() == true) {
        kDebug() << "Settings changed";
        configManager->updateDefaultValues();
        settingsChangedSlot();
    }
}

void WebcamDialog::updateWidgetsDefault()
{
    kDebug() << "updates widget!";
    KConfigDialog::updateWidgetsDefault();
    configManager->updateWidgetsDefault();
}
