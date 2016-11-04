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

#include <KAboutData>
#include <KLocalizedString>
#include <KDeclarative/KDeclarative>
#include <QCommandLineParser>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QApplication>

#include "kamoso.h"
#include "kamosodirmodel.h"
#include "previewfetcher.h"
#include "kamosoSettings.h"
#include "whitewidgetmanager.h"

int main(int argc, char *argv[])
{
    KAboutData about("kamoso", i18n("Kamoso"), "3.1.0", i18n("Utility for taking photos and videos using a webcam"),
                KAboutLicense::GPL, i18n("(C) 2008-2015 Alex Fiestas and Aleix Pol"));

    about.addAuthor( i18n("Aleix Pol Gonzalez"), i18n("Semaphore hacker"), "aleixpol@kde.org" );
    about.addAuthor( i18n("Alex Fiestas"), i18n("Coffee drinker"), "afiestas@kde.org" );
    QApplication app(argc, argv);

    {
        QCommandLineParser parser;
        about.setupCommandLine(&parser);
        parser.process(app);
        about.processCommandLine(&parser);
    }

    QQmlApplicationEngine* engine = new QQmlApplicationEngine(&app);
    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine);
    kdeclarative.setTranslationDomain("kamoso");
    kdeclarative.setupBindings();

    qmlRegisterType<KamosoDirModel>("org.kde.kamoso", 3, 0, "DirModel");
    qmlRegisterType<PreviewFetcher>("org.kde.kamoso", 3, 0, "PreviewFetcher");
    qmlRegisterUncreatableType<KJob>("org.kde.kamoso", 3, 0, "KJob", "you're not supposed to do that");

    //Make these qml singletons
    engine->rootContext()->setContextProperty("config", Settings::self());
    engine->rootContext()->setContextProperty("whites", new WhiteWidgetManager(&app));
    engine->rootContext()->setContextProperty("webcam", new Kamoso());
    engine->load(QUrl("qrc:/qml/Main.qml"));

    return app.exec();
}
