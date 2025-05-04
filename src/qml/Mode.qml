/*
    SPDX-FileCopyrightText: 2015 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import org.kde.kirigami

Action {
    id: modeAction
    property string mimes: ""
    property string name: ""
    property string modeInfo: ""
    property string nameFilter: ""

    default property var things: []
}
