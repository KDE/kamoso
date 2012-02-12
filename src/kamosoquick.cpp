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

#include "kamosoquick.h"
#include "kamosodirmodel.h"
#include "kamososettings.h"
#include "whitewidgetmanager.h"
#include <kdeclarative.h>
#include <qdeclarative.h>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#include <QGst/Ui/GraphicsVideoSurface>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>

KamosoQuick::KamosoQuick(QWidget* parent)
    : QDeclarativeView(parent)
{
    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    //binds things like kconfig and icons
    kdeclarative.setupBindings();

    QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(this);
    engine()->rootContext()->setContextProperty(QLatin1String("videoSurface1"), surface);

    QGst::PipelinePtr pipeline = QGst::Pipeline::create();
    QGst::ElementPtr src = QGst::ElementFactory::make("v4l2src");
    QGst::ElementPtr color = QGst::ElementFactory::make("ffmpegcolorspace");

    pipeline->add(src, color, surface->videoSink());

    src->link(color);

    color->link(surface->videoSink());

    pipeline->setState(QGst::StatePlaying);

    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    engine()->rootContext()->setContextProperty("settings", new KamosoSettings);
    engine()->rootContext()->setContextProperty("whites", new WhiteWidgetManager(this));
    setResizeMode(SizeRootObjectToView);
    setSource(QUrl("qrc:/qml/Main.qml"));
//     Q_ASSERT(errors().isEmpty());
}
