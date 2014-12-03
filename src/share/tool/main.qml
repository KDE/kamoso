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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import org.kde.kamoso.share 3.0

ApplicationWindow
{
    id: window
    visible: true
    property string mimetype: ""

    ScrollView {
        ListView {
            header: Label {
                text: window.mimetype
            }

            model: ShareAlternativesModel {
                id: altsModel
                mimeTypes: [ window.mimetype ]
            }
            delegate: RowLayout {
                Label {
                    text: display
                }
                Button {
                    text: i18n("Use")
                    onClicked: {
                        var job = altsModel.createJob(index);
                    }
                }
            }
        }
    }
}