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

#include "fakekipiinterface.h"
#include "fakeinfoshared.h"
#include "fakeimagecollectionshared.h"
#include <libkipi/uploadwidget.h>
#include <libkipi/imagecollectionshared.h>
#include <libkipi/imageinfo.h>
#include <libkipi/imageinfoshared.h>
#include <libkipi/plugin.h>
#include <libkipi/interface.h>
#include <libkipi/pluginloader.h>
#include <libkipi/imagecollection.h>
#include <libkipi/imagecollectionselector.h>

struct FakeKIPIInterfacePrivate {
    KIPI::PluginLoader* pluginLoader;
    FakeImageCollectionShared* collectionShared;
};

FakeKIPIInterface::FakeKIPIInterface(KUrl::List& kurlList)
:KIPI::Interface(0)
, d(new FakeKIPIInterfacePrivate) {
    d->collectionShared = new FakeImageCollectionShared(kurlList);
}

FakeKIPIInterface::~FakeKIPIInterface() {
    delete d;
}

KIPI::ImageCollection FakeKIPIInterface::currentAlbum() {
    return KIPI::ImageCollection(d->collectionShared);
}

KIPI::ImageCollection FakeKIPIInterface::currentSelection() {
    return KIPI::ImageCollection(d->collectionShared);
}

QList<KIPI::ImageCollection> FakeKIPIInterface::allAlbums() {
    QList<KIPI::ImageCollection> list;
    list << currentSelection();
    return list;
}

KIPI::ImageInfo FakeKIPIInterface::info(const KUrl& url) {
    return KIPI::ImageInfo(new FakeInfoShared(this,url));
}

bool FakeKIPIInterface::addImage(const KUrl&, QString& err)
{
    Q_UNUSED(err)

    return true;
}
void FakeKIPIInterface::delImage( const KUrl& )
{

}
void FakeKIPIInterface::refreshImages( const KUrl::List& urls )
{
    Q_UNUSED(urls)
}

KIPI::ImageCollectionSelector* FakeKIPIInterface::imageCollectionSelector(QWidget *parent) {
    return new KIPI::ImageCollectionSelector(parent);
}

KIPI::UploadWidget* FakeKIPIInterface::uploadWidget(QWidget *parent) {
    return (new KIPI::UploadWidget(parent));
}

int FakeKIPIInterface::features() const {
    return KIPI::HostAcceptNewImages;
}
