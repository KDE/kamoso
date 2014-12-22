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

import QtQuick 2.2

Loader {
    property QtObject job
    signal accepted()

    function cancel() {
        job.destroy();
    }

    id: loader

    Component.onCompleted: {
        setSource(job.configSourceCode, job.data)
    }
    onItemChanged: {
        for(var i in job.neededArguments) {
            var arg = job.neededArguments[i]
            if (arg in loader.item) {
                item[arg+"Changed"].connect(dataHasChanged);
            } else
                console.warn("property not found", arg);
        }
    }

    function dataHasChanged()
    {
        var jobData = job.data;
        for(var i in job.neededArguments) {
            var arg = job.neededArguments[i]
            if (arg in loader.item) {
                jobData[arg] = loader.item[arg];
            } else
                console.warn("property not found", arg);
        }
        job.data = jobData;
    }
}
