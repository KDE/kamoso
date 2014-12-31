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

/**
 * @brief Base class to implement by plugins
 *
 * This file shouldn't be included by any application.
 *
 * The plugin properties should be specified in the .desktop/.json file.
 *
 * There will be 2 files specifying the behavior of the plugin: the *PluginType.json
 * files and the plugin metadata itself.
 *
 * The plugin type will be identified by the file name. It will specify:
 *  - X-Purpose-InboundArguments defines the arguments the application must provide.
 *  - X-Purpose-OutboundArguments defines the arguments the plugin must provide by
 * the end of the execution.
 *
 * In the plugin metadata we will define:
 *  - X-Purpose-PluginTypes defines the purposes tackled by the plugin
 *  - X-Purpose-Constraints defines some conditions under the plugin is useful, considering
 * the provided inboundArguments. For example, the youtube export plugin will specify
 * mimeType:video/* as a constraint, because it's not interested in uploading images.
 *  - X-Purpose-Configuration provides a list of extra arguments that the plugin will need.
 * Ideally everything should be in the plugin type but sometimes we can only wish. This allows
 * the opportunity to the application to let the user add the missing data.
 */
class Q_DECL_EXPORT PluginBase : public QObject
{
Q_OBJECT
public:
    PluginBase(QObject* parent = nullptr);
    virtual ~PluginBase();

    /** @returns the job that will perform the share of the specified @p data.*/
    virtual Job* share() const = 0;
};

}

Q_DECLARE_INTERFACE(Purpose::PluginBase, "org.kde.purpose")

#endif
