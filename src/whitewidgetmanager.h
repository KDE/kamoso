/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef WHITEWIDGETMANAGER_H
#define WHITEWIDGETMANAGER_H

#include <QObject>
#include "whitewidget.h"

class QPropertyAnimation;
class WhiteWidgetManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    public:
        explicit WhiteWidgetManager(QObject* parent);
        ~WhiteWidgetManager();

        qreal opacity() const;
        void setOpacity(qreal op);

    public Q_SLOTS:
        void showAll();
        void hideAll();

    private:
        void createWhiteWidgets();
        QList<WhiteWidget*> whitewidgetList;
        QPropertyAnimation* m_timer;
};

#endif //WHITEWIDGETMANAGER
