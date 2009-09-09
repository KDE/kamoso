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

#include "photoshootmode.h"
#include <KIcon>
#include <KLocalizedString>
#include <QAction>
#include "kamoso.h"
#include <QPushButton>

PhotoShootMode::PhotoShootMode(Kamoso* camera)
	: ShootMode(camera)
{
	QAction* flash=new QAction(KIcon("weather-clear"), i18n("Use flash"), this);
	flash->setCheckable(true);
	flash->setChecked(controller()->isFlashEnabled());
	connect(flash, SIGNAL(triggered(bool)), controller(), SLOT(setFlashEnabled(bool)));
	
	mActions += flash;
}

QWidget* PhotoShootMode::mainAction()
{
	QPushButton* action = new QPushButton(controller());
	action->setIcon(icon());
	action->setIconSize(QSize(32,32));
	action->setToolTip(name());
	
	connect(action, SIGNAL(clicked()), controller(), SLOT(startCountdown()));
	return action;
}

QIcon PhotoShootMode::icon() const
{
	return KIcon("webcamreceive");
}

QString PhotoShootMode::name() const
{
	return i18n("Take pictures");
}

QStringList PhotoShootMode::thumbnailsViewMimeTypes() const
{
	return QStringList() << "image/png";
}