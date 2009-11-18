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

#ifndef YOUTUBEMANAGER_H
#define YOUTUBEMANAGER_H

#include <QObject>

struct Private;

/**
*YoutubeManager class is a dispatcher of YoutubeJobs.
*This clases only cares about authentication, so no other checks
*are done
*
*No YoutubeJobs will be dispatched if YoutubeManager doesn't have
*username and password, no matters if the authToken is valid.
*/

class YoutubeManager : public QObject
{
public:
	YoutubeManager(QObject* parent = 0);
	YoutubeManager(QByteArray &username, QByteArray &password, QObject* parent = 0);
	YoutubeManager(QByteArray &authToken, QObject* parent = 0);

	void setName(QByteArray &username);
	void setPassword(QByteArray &password);
	void setAuthToken(QByteArray &token);
	QByteArray& name() const;
	QByteArray& password() const;
	QByteArray& authToken() const;
private:
	Private* p;
};

#endif // YOUTUBEMANAGER_H
