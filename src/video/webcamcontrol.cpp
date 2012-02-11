/*************************************************************************************
 *  Copyright (C) 2012 by Alejandro Fiestas Olivares <afiestaso@kde.org>             *
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


#include "webcamcontrol.h"

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

#include <QGst/Ui/GraphicsVideoSurface>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>

WebcamControl::WebcamControl(QDeclarativeView* view)
{
    QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(view);
    view->engine()->rootContext()->setContextProperty(QLatin1String("videoSurface1"), surface);

    QGst::PipelinePtr pipeline = QGst::Pipeline::create();
    QGst::ElementPtr src = QGst::ElementFactory::make("v4l2src");
    QGst::ElementPtr color = QGst::ElementFactory::make("ffmpegcolorspace");

    pipeline->add(src, color, surface->videoSink());

    src->link(color);

    color->link(surface->videoSink());

    pipeline->setState(QGst::StatePlaying);
}

WebcamControl::~WebcamControl()
{

}

