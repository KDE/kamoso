/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "pagewebcamconfigmanager.h"
#include <QDebug>


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
	qDebug() << "Setting default values to sliders";
	d->pageWebcam->brightnessSlider->setValue(100);
	d->pageWebcam->contrastSlider->setValue(100);
	d->pageWebcam->saturationSlider->setValue(100);
	d->pageWebcam->gammaSlider->setValue(100);
	d->pageWebcam->hueSlider->setValue(0);
}