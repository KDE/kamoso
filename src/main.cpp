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

#include <kaboutdata.h>
#include <QCommandLineParser>
#include <klocalizedstring.h>
#include "video/webcamcontrol.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kamoso");
    {
        KAboutData about("kamoso", i18n("Kamoso"), "3.2.3", i18n("Utility for taking photos and videos using a webcam"),
                         KAboutLicense::GPL, i18n("(C) 2008-2015 Alex Fiestas and Aleix Pol"));

        about.addAuthor( i18n("Aleix Pol Gonzalez"), i18n("Semaphore hacker"), "aleixpol@kde.org" );
        about.addAuthor( i18n("Alex Fiestas"), i18n("Coffee drinker"), "afiestas@kde.org" );
        KAboutData::setApplicationData(about);
        QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kamoso"), app.windowIcon()));

        QCommandLineParser parser;
        about.setupCommandLine(&parser);
        parser.process(app);
        about.processCommandLine(&parser);
    }

    WebcamControl webcamControl;
    if (!webcamControl.play()) {
        qWarning("Unrecoverable error occurred when initializing webcam. Exiting.");
        QApplication::exit(1);
        return 1;
    }

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &webcamControl, &WebcamControl::stop);

    return app.exec();
}
