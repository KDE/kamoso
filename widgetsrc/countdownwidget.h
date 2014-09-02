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

#ifndef COUNTDOWNWIDGET_H
#define COUNTDOWNWIDGET_H

#include <QWidget>

class QTimeLine;
class CountdownWidget : public QWidget
{
Q_OBJECT
    public:
        CountdownWidget(QWidget* parent=0);

        QSize sizeHint() const { return QSize(100,100); }

        virtual void paintEvent (QPaintEvent*);
        virtual void hideEvent(QHideEvent* );

        void start(int timeInterval);
        void stop();

    private Q_SLOTS:
        void tick(qreal progress);

    Q_SIGNALS:
        void finished();

    private:
        qreal mProgress;
        QTimeLine* mTimer;
};

#endif // COUNTDOWNWIDGET_H
