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

#ifndef SHOOTMODE_H
#define SHOOTMODE_H

#include <QtGui/QWidget>

class Kamoso;
class ShootMode : public QObject
{
Q_OBJECT
    public:
        ShootMode(Kamoso* camera);
        virtual ~ShootMode();

        /** @returns the name of the shooting mode */
        virtual QString name() const=0;

        /** @returns the icon of the shooting mode */
        virtual QIcon icon() const=0;

        /** @returns the main action associated to the shooting mode */
        virtual QWidget* mainAction()=0;

        virtual QStringList thumbnailsViewMimeTypes() const=0;
        /** @returns some more actions associated to the shooting mode */
        virtual QList<QAction*> actions()=0;

        /** Deactivate the mode, is like a destructor */
        virtual void deactivate()=0;

        /** */
        Kamoso* controller() const;

    private:
        Kamoso* mController;
};

#endif // SHOOTMODE_H
