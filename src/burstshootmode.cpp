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

#include "burstshootmode.h"
#include "countdownwidget.h"
#include <KIcon>
#include <KLocalizedString>
#include <QAction>
#include "kamoso.h"
#include <QPushButton>
#include <settings.h>

BurstShootMode::BurstShootMode(Kamoso* camera)
	: ShootMode(camera)
{}

QWidget* BurstShootMode::mainAction()
{
	m_action = new QPushButton(controller());
	m_action->setIcon(icon());
	m_action->setIconSize(QSize(32,32));
	m_action->setToolTip(name());
	m_action->setCheckable(true);
	connect(m_action, SIGNAL(clicked()), this, SLOT(startBurstMode()));
	connect(controller()->countdown(),SIGNAL(finished()),this,SLOT(startBurstMode()));
	return m_action;
}

void BurstShootMode::startBurstMode()
{
	if(m_action->isChecked() == true)
	{
		int interval = Settings::photoTime()/3;
		if(interval < 1){
			interval = 1;
		}
		controller()->startCountdown(interval);
	}
}

QIcon BurstShootMode::icon() const
{
	return KIcon("code-block");
}

QString BurstShootMode::name() const
{
	return i18n("Take pictures");
}

QStringList BurstShootMode::thumbnailsViewMimeTypes() const
{
	return QStringList() << "image/png";
}