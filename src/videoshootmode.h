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

#ifndef VIDEOSHOOTMODE_H
#define VIDEOSHOOTMODE_H

#include "shootmode.h"

class VideoShootMode : public ShootMode
{
Q_OBJECT
    public:
        VideoShootMode(Kamoso* camera);
        virtual void deactivate();
        virtual QList<QAction*> actions() { return mActions; }
        virtual QWidget* mainAction();
        virtual QIcon icon() const;
        virtual QString name() const;
        virtual QStringList thumbnailsViewMimeTypes() const;
    public slots:
        void videoPressed(bool pressed);
    private:
        QList<QAction*> mActions;
};

#endif // VIDEOSHOOTMODE_H
