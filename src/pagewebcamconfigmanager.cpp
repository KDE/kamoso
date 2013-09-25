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

#include "pagewebcamconfigmanager.h"

#include <KDebug>

struct Private
{
    Ui::webcamConfigWidget* pageWebcam;
    int brightness,contrast,saturation,gamma,hue;
};

PageWebcamConfigManager::PageWebcamConfigManager(Ui::webcamConfigWidget* pageWebcam):d(new Private)
{
    d->pageWebcam = pageWebcam;
    updateDefaultValues();
}

bool PageWebcamConfigManager::hasChanged()
{
    if(d->brightness != d->pageWebcam->brightnessSlider->value()) {
        return true;
    }
    if(d->contrast != d->pageWebcam->contrastSlider->value()) {
        return true;
    }
    if(d->saturation != d->pageWebcam->saturationSlider->value()) {
        return true;
    }
    if(d->gamma != d->pageWebcam->gammaSlider->value()) {
        return true;
    }
    if(d->hue != d->pageWebcam->hueSlider->value()) {
        return true;
    }

    return false;
}


void PageWebcamConfigManager::updateDefaultValues()
{
    d->brightness = d->pageWebcam->brightnessSlider->value();
    d->contrast = d->pageWebcam->contrastSlider->value();
    d->saturation = d->pageWebcam->saturationSlider->value();
    d->gamma = d->pageWebcam->gammaSlider->value();
    d->hue = d->pageWebcam->hueSlider->value();
}


void PageWebcamConfigManager::updateWidgetsDefault()
{
    kDebug() << "Setting default values to sliders";
    d->pageWebcam->brightnessSlider->setValue(0);
    d->pageWebcam->contrastSlider->setValue(100);
    d->pageWebcam->saturationSlider->setValue(100);
    d->pageWebcam->gammaSlider->setValue(100);
    d->pageWebcam->hueSlider->setValue(0);
}
