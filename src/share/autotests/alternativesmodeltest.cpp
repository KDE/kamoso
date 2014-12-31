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

#include <qtest.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QSignalSpy>

#include "alternativesmodeltest.h"
#include <share/alternativesmodel.h>
#include <share/job.h>

QTEST_MAIN(AlternativesModelTest)

void AlternativesModelTest::runJobTest()
{
    Purpose::AlternativesModel model;

    const QString tempfile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/purposetest";
    QJsonObject input = QJsonObject {
        {"urls", QJsonArray {"http://kde.org"} },
        {"mimeType", "dummy/thing" }
    };
    model.setInputData(input);
    //TODO: should probably make a separate plugin type for testing, at the moment it's not testable without installing
    model.setPluginType("Export");
    QCOMPARE(model.rowCount(), 1); //     NOTE: we are assuming this plugin is the dummy plugin
    Purpose::Job* job = model.createJob(0);
    QVERIFY(job);
    QVERIFY(!job->isReady());
    input.insert("destinationPath", tempfile),
    job->setData(input);
    QVERIFY(job->isReady());
    job->start();

    QSignalSpy s(job, &KJob::finished);
    QVERIFY(s.wait());
    QVERIFY(QFile::remove(tempfile));
}
