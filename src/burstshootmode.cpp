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

#include "burstshootmode.h"
#include "countdownwidget.h"
#include <KIcon>
#include <KLocalizedString>
#include <QAction>
#include "kamoso.h"
#include <QPushButton>

BurstShootMode::BurstShootMode(Kamoso* camera)
	: ShootMode(camera)
{}
BurstShootMode::~BurstShootMode()
{
	delete m_action;
}
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
		controller()->startCountdown();
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

