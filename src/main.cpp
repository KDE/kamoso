/*
    SPDX-FileCopyrightText: 2008-2011 Aleix Pol <aleixpol@kde.org>
    SPDX-FileCopyrightText: 2008-2011 Alex Fiestas <alex@eyeos.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <kaboutdata.h>
#include <QCommandLineParser>
#include <klocalizedstring.h>
#include "video/webcamcontrol.h"
#include <QApplication>
#include <QIcon>

#include "kamoso_version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    KLocalizedString::setApplicationDomain("kamoso");
    {
        KAboutData about("kamoso", i18n("Kamoso"), KAMOSO_VERSION_STRING, i18n("Utility for taking photos and videos using a webcam"),
                         KAboutLicense::GPL, i18n("(C) 2008-2015 Alex Fiestas and Aleix Pol"));

        about.addAuthor( i18n("Aleix Pol Gonzalez"), i18n("Semaphore hacker"), "aleixpol@kde.org" );
        about.addAuthor( i18n("Alex Fiestas"), i18n("Coffee drinker"), "afiestas@kde.org" );
        about.addCredit( i18n("caseymoura"), i18n("Uploaded the shutter sound: https://freesound.org/people/caseymoura/sounds/445482/"), {} );
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
