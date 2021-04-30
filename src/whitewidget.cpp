/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "whitewidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <klocalizedstring.h>

WhiteWidget::WhiteWidget()
    : QQuickView()
{
    setSource(QUrl("qrc:/qml/White.qml"));
}
