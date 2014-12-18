/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
 *  Copyright (C) 2013      by Michael G. Hansen <mike@mghansen.de>                  *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/


#include "kamoso.h"

#include "settings.h"
#include "video/webcamcontrol.h"
#include "devicemanager.h"

#include <QtCore/QFile>

Kamoso::Kamoso(WebcamControl *webcamControl)
{
    m_webcamControl = webcamControl;
    connect(m_webcamControl, SIGNAL(photoTaken(QString)), this, SIGNAL(photoTaken(QString)));
}

Kamoso::~Kamoso()
{

}

const QString Kamoso::takePhoto()
{
    QUrl photoPlace = Settings::saveUrl();
    photoPlace.setPath( photoPlace.toLocalFile() +'/' + QStringLiteral("picture_1.jpg") );

    QFile file(photoPlace.toLocalFile());
    while(file.exists()) {
        autoincFilename(photoPlace);
        file.setFileName(photoPlace.toLocalFile());
    }

    m_webcamControl->takePhoto(photoPlace);

    return photoPlace.toDisplayString();
}

void Kamoso::startRecording()
{
    m_webcamControl->startRecording();
}

void Kamoso::stopRecording()
{
    QUrl photoPlace = Settings::saveUrl();
    photoPlace.setPath( photoPlace.toLocalFile() +'/' + QStringLiteral("video_1.mkv") );

    QFile file(photoPlace.toLocalFile());
    while(file.exists()) {
        autoincFilename(photoPlace);
        file.setFileName(photoPlace.toLocalFile());
    }

    QFile::rename(m_webcamControl->stopRecording(), photoPlace.toLocalFile());

    m_webcamControl->play(DeviceManager::self()->playingDevice());
}

void Kamoso::autoincFilename(QUrl &filename)
{
    // Extract the filename from the path
    QString name= filename.fileName();

    // If the name contains a number then increment it
    QRegExp numSearch( "(^|[^\\d])(\\d+)" ); // we want to match as far left as possible, and when the number is at the start of the name

    // Does it have a number?
    int start = numSearch.lastIndexIn( name );
    if (start != -1) {
        // It has a number, increment it
        start = numSearch.pos( 2 ); // we are only interested in the second group
        QString numAsStr = numSearch.cap(2);
        QString number = QString::number( numAsStr.toInt() + 1 );
        number = number.rightJustified( numAsStr.length(), '0' );
        name.replace( start, numAsStr.length(), number );
    }
    else {
        // no number
        start = name.lastIndexOf('.');
        if (start != -1) {
            // has a . somewhere, e.g. it has an extension
            name.insert(start, '1');
        }
        else {
            // no extension, just tack it on to the end
            name += '1';
        }
    }

    //Rebuild the path
    filename.setPath( filename.adjusted(QUrl::RemoveFilename).toLocalFile() + name );
}
