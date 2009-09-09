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

#include "photoshootmode.h"
#include <KIcon>
#include <KLocalizedString>
#include <QAction>
#include "kamoso.h"
#include <QPushButton>

PhotoShootMode::PhotoShootMode(Kamoso* camera)
	: ShootMode(camera)
{
	QAction* flash=new QAction(KIcon("weather-clear"), i18n("Use Flash"), this);
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
