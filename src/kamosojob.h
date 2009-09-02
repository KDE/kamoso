
class KIcon;/*
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

#ifndef KAMOSOJOB_H
#define KAMOSOJOB_H

#include <KJob>
class KUrl;

class KDE_EXPORT KamosoJob : public KJob
{
	Q_OBJECT
	public:
		KamosoJob(QObject* parent = 0);
		virtual void start()=0;
		virtual QList<KUrl> urls() const=0;
		virtual KIcon icon() const=0;
};

#endif // KAMOSOJOB_H
