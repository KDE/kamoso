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

#include <QApplication>
#include <KAboutData>
#include <QCommandLineParser>
#include <QMimeDatabase>
#include <KLocalizedString>
#include <kdeclarative/kdeclarative.h>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include "qqml.h"
#include <share/alternativesmodel.h>
#include <share/job.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    KAboutData data("sharetool", i18n("Share Tool"), "1.0", i18n("Share random information"), KAboutLicense::GPL);
    data.addAuthor("Aleix Pol i Gonzalez", i18n("Implementation"), "aleixpol@kde.org");
    KAboutData::setApplicationData(data);

    QJsonObject inputData;
    QStringList files;
    {
        QCommandLineParser parser;
        parser.addPositionalArgument("files", i18n("Files to share"), "[files...]");
        parser.addOption(QCommandLineOption("data", i18n("Data tuple to initialize the process with"), "json"));

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
        if (parser.isSet("data")) {
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(parser.value("data").toLatin1(), &error);
            if (error.error) {
                qCritical() << qPrintable(i18n("Error in the data argument formatting: %1 at %2", error.errorString(), error.offset));
                parser.showHelp(2);
            } else if (!doc.isObject()) {
                qCritical() << qPrintable(i18n("Error in the data argument type, it should be a json object."));
                parser.showHelp(3);
            }
            inputData = doc.object();
        }
    }

    QMimeType common;
    QJsonArray urls;
    {
        QMimeDatabase db;
        for(const QString& file: files) {
            const QUrl url = QUrl::fromUserInput(file);
            QMimeType type = db.mimeTypeForUrl(url);
            if (!common.isValid())
                common = type;
            else if(common.inherits(type.name())) {
                common = type;
            } else if(type.inherits(common.name())) {
                ;
            } else {
                common = db.mimeTypeForName("application/octet-stream");
            }
            urls += url.toString();
        }
    }
    Q_ASSERT(common.isValid());

    qmlRegisterType<Purpose::AlternativesModel>("org.kde.purpose", 1, 0, "PurposeAlternativesModel");
    qmlRegisterUncreatableType<Purpose::Job>("org.kde.purpose", 1, 0, "PurposeJob", "You're not supposed to mess with this yo");

    QQmlApplicationEngine engine;
    KDeclarative::KDeclarative decl;
    decl.setDeclarativeEngine(&engine);
    decl.setupBindings();
    engine.load(QUrl("qrc:/main.qml"));

    inputData.insert("urls", urls);
    inputData.insert("mimeType", common.name());
    engine.rootObjects().first()->setProperty("inputData", inputData);

    return app.exec();
}
