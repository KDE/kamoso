/*
    Copyright (C) 2014 Aleix Pol i Gonzalez <aleixpol@blue-systems.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.2

ColumnLayout {
    property alias videoDesc: description.text
    property alias videoTitle: title.text
    property alias videoTags: tags.text

    Label { text: i18n("Title:") }
    TextField {
        id: title
        Layout.fillWidth: true
        placeholderText: i18n("Enter a title for the video...")
    }

    Label { text: i18n("Tags:") }
    TextField {
        id: tags
        Layout.fillWidth: true
        placeholderText: i18n("KDE, Kamoso")
    }

    Label { text: i18n("Description:") }
    TextArea {
        id: description
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
