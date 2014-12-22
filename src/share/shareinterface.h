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

#ifndef SHAREINTERFACE_H
#define SHAREINTERFACE_H

#include <KJob>
#include <QJsonObject>
#include <QMimeData>
#include <QUrl>

#define EXPORT_SHARE_VERSION K_EXPORT_PLUGIN_VERSION(2)

class ShareJobPrivate;

/**
 * @brief Job that will actually perform the sharing
 *
 * When start is called, the sharing process will start and when the job
 * emits finished, we'll know it's over.
 */
class Q_DECL_EXPORT ShareJob : public KJob
{
Q_OBJECT
/**
 * Represents the data the job will have available to perform its task
 */
Q_PROPERTY(QJsonObject data READ data WRITE setData NOTIFY dataChanged)

/**
 * Tells whether there's still information to be provided, to be able to run
 * the job.
 *
 * @sa X-KamosoShare-MandatoryArguments and X-KamosoShare-AdditionalArguments
 */
Q_PROPERTY(bool isReady READ isReady NOTIFY dataChanged)

/**
 * Specifies the qml source code to be used, to configure the current job.
 *
 * @sa ShareWizard QtQuick component
 */
Q_PROPERTY(QUrl configSourceCode READ configSourceCode CONSTANT)

/**
 * Specifies the arguments the config file and the job will be sharing
 */
Q_PROPERTY(QStringList neededArguments READ neededArguments CONSTANT)
public:
    ShareJob(QObject* parent = 0);
    virtual ~ShareJob();

    void setData(const QJsonObject& data);
    QJsonObject data() const;

    bool isReady() const;
    QStringList neededArguments() const;
    virtual QUrl configSourceCode() const = 0;

    /**
     * @internal
     */
    void setInboundArguments(const QStringList& args);

    /**
     * @internal
     */
    void setConfigurationArguments(const QStringList& args);


Q_SIGNALS:
    void output(const QVariant& output);
    void dataChanged();

private:
    Q_DECLARE_PRIVATE(ShareJob);
    ShareJobPrivate *const d_ptr;
};

class Q_DECL_EXPORT SharePlugin : public QObject
{
Q_OBJECT
public:
    /**
     * The plugin properties should be specified in the .desktop/.json file.
     *
     * There we will specify under what circumstances the plugin is useful.
     * Fields:
     *  - X-KamosoShare-MimeType defines the accepted mimetype files (default ("*")
     *  - X-KamosoShare-RequiredArguments defines the arguments the application needs to provide so the plugin is available (default "Urls")
     *  - X-KamosoShare-AdditionalArguments defines the arguments the plugin can take, if not filled the plugin will request interaction with ::needInteraction(QUrl) signal.
     */

    SharePlugin(QObject* parent = nullptr);
    virtual ~SharePlugin();

    /** @returns the job that will perform the share of the specified @p data.*/
    virtual ShareJob* share() const = 0;
};

Q_DECLARE_INTERFACE(SharePlugin, "org.kde.kamoso.share")

#endif
