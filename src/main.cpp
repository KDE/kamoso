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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include "kamosoquick.h"

#include <QGst/Init>
int main(int argc, char *argv[])
{
    KAboutData about("kamoso", 0, ki18n(("Kamoso")), "1.90", ki18n("Webcam picture retriever"),
                KAboutData::License_GPL, ki18n("(C) 2008-2012 Alex Fiestas and Aleix Pol"));
    about.addAuthor( ki18n("Aleix Pol Gonzalez"), ki18n("Semaphore hacker"), "aleixpol@kde.org" );
    about.addAuthor( ki18n("Alex Fiestas"), ki18n("Coffee drinker"), "alex@eyeos.org" );
    KCmdLineArgs::init(argc, argv, &about);
    QGst::init(&argc, &argv);
    KApplication app;

    KamosoQuick* kamosoMain = new KamosoQuick;
    kamosoMain->show();

    return app.exec();
}
