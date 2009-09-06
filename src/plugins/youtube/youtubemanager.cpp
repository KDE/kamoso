/*************************************************************************************
 *  Copyright (C) 2008-2009 by Aleix Pol <aleixpol@gmail.com>                        *
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

#include "youtubemanager.h"
#include <QDebug>
#include <KIO/Job>
#include <KUrl>

YoutubeManager::YoutubeManager(QByteArray username,QByteArray password, QByteArray developerKey)
{
	qDebug() << username;
	qDebug() << password;
	setUsername(username);
	setPassword(password);
	setDeveloperKey(developerKey);
}




void YoutubeManager::setUsername(QByteArray username)
{
	m_username = username;
}

void YoutubeManager::setPassword(QByteArray password)
{
	m_password = password;
}

void YoutubeManager::setDeveloperKey(QByteArray developerKey)
{
	m_developerKey = developerKey;
}

void YoutubeManager::setAuthToken(QByteArray authToken)
{
	m_authToken = authToken;
}

QByteArray YoutubeManager::username() const
{
	return m_username;
}

QByteArray YoutubeManager::password() const
{
	return m_password;
}

QByteArray YoutubeManager::developerKey() const
{
	return m_developerKey;
}

QByteArray YoutubeManager::authToken() const
{
	return m_authToken;
}