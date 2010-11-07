/*************************************************************************************
 *  Copyright (C) 2008-2010 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2010 by Alex Fiestas <alex@eyeos.org>                         *
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

#include "fakeinfoshared.h"

FakeInfoShared::FakeInfoShared(FakeKIPIInterface* interface, const KUrl& url) : KIPI::ImageInfoShared(interface,url)
{

}

void FakeInfoShared::delAttributes(const QStringList& )
{

}

void FakeInfoShared::addAttributes(const QMap< QString, QVariant >& )
{

}

void FakeInfoShared::clearAttributes()
{

}

QMap< QString, QVariant > FakeInfoShared::attributes()
{
    return QMap<QString,QVariant>();
}

void FakeInfoShared::setDescription(const QString& )
{

}

QString FakeInfoShared::description()
{
    return "Took with kamoso";
}
