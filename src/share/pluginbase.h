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

#ifndef PURPOSE_PLUGINBASE_H
#define PURPOSE_PLUGINBASE_H

#include <QObject>
#include "job.h"

#define EXPORT_SHARE_VERSION K_EXPORT_PLUGIN_VERSION(2)

namespace Purpose
{

/// only to be used in plugin implementations
class Q_DECL_EXPORT PluginBase : public QObject
{
Q_OBJECT
public:
#warning Update API documentation
    /**
     * The plugin properties should be specified in the .desktop/.json file.
     *
     * There we will specify under what circumstances the plugin is useful.
     * Fields:
     *  - X-Purpose-MimeType defines the accepted mimetype files (default ("*")
     *  - X-Purpose-RequiredArguments defines the arguments the application needs to provide so the plugin is available (default "Urls")
     *  - X-Purpose-AdditionalArguments defines the arguments the plugin can take, if not filled the plugin will request interaction with ::needInteraction(QUrl) signal.
     */

    PluginBase(QObject* parent = nullptr);
    virtual ~PluginBase();

    /** @returns the job that will perform the share of the specified @p data.*/
    virtual Job* share() const = 0;
};

}

Q_DECLARE_INTERFACE(Purpose::PluginBase, "org.kde.purpose")

#endif
