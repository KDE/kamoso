/************************************************************************************
 * Copyright (C) 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>                *
 *                                                                                  *
 * This program is free software; you can redistribute it and/or                    *
 * modify it under the terms of the GNU General Public License                      *
 * as published by the Free Software Foundation; either version 2                   *
 * of the License, or (at your option) any later version.                           *
 *                                                                                  *
 * This program is distributed in the hope that it will be useful,                  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 * GNU General Public License for more details.                                     *
 *                                                                                  *
 * You should have received a copy of the GNU General Public License                *
 * along with this program; if not, write to the Free Software                      *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 ************************************************************************************/

#include <QGuiApplication>
#include <KAboutData>
#include <QCommandLineParser>
#include <QMimeDatabase>
#include <KLocalizedString>
#include <kdeclarative/kdeclarative.h>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "qqml.h"
#include "sharealternativesmodel.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    KAboutData data("sharetool", i18n("Share Tool"), "1.0", i18n("Share random information"), KAboutLicense::GPL);
    data.addAuthor("Aleix Pol i Gonzalez", i18n("Implementation"), "aleixpol@kde.org");
    KAboutData::setApplicationData(data);

    QStringList files;
    {
        QCommandLineParser parser;
        parser.addPositionalArgument("files", i18n("Files to share"), "[files...]");

        data.setupCommandLine(&parser);
        parser.addHelpOption();
        parser.addVersionOption();
        parser.process(app);
        data.processCommandLine(&parser);

        files = parser.positionalArguments();
        if (files.isEmpty()) {
            qCritical() << qPrintable(i18n("Must specify some files to share"));
            parser.showHelp(1);
        }
    }

    QMimeType common;
    {
        QMimeDatabase db;
        for(const QString& file: files) {
            QMimeType type = db.mimeTypeForFile(file);
            if (!common.isValid())
                common = type;
            else if(common.inherits(type.name())) {
                common = type;
            } else if(type.inherits(common.name())) {
                ;
            } else {
                common = db.mimeTypeForName("application/octet-stream");
            }
        }
    }
    Q_ASSERT(common.isValid());

    qmlRegisterType<ShareAlternativesModel>("org.kde.kamoso.share", 3, 0, "ShareAlternativesModel");
    qmlRegisterUncreatableType<ShareJob>("org.kde.kamoso.share", 3, 0, "ShareJob", "You're not supposed to mess with this yo");

    QQmlApplicationEngine engine;
    KDeclarative::KDeclarative decl;
    decl.setDeclarativeEngine(&engine);
    decl.setupBindings();
    engine.load(QUrl("qrc:/main.qml"));
    engine.rootObjects().first()->setProperty("mimetype", common.name());

    return app.exec();
}
