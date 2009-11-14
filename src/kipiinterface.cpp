/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "kipiinterface.h"
#include "infoshared.h"
#include "kamosoimagecollectionshared.h"
#include "kamoso.h"
#include <libkipi/uploadwidget.h>
#include <libkipi/imagecollectionshared.h>
#include <libkipi/imageinfo.h>
#include <libkipi/imageinfoshared.h>
#include <libkipi/plugin.h>
#include <libkipi/interface.h>
#include <libkipi/pluginloader.h>
#include <libkipi/imagecollection.h>
#include <libkipi/imagecollectionselector.h>
#include <libkipi/imagecollection.h>
#include <libkipi/imageinfo.h>

struct KIPIInterfacePrivate {
	Kamoso *kamoso;
	KIPI::PluginLoader* pluginLoader;
	KamosoImageCollectionShared* collectionShared;
};

KIPIInterface::KIPIInterface(Kamoso* kamoso)
:KIPI::Interface(kamoso)
, d(new KIPIInterfacePrivate) {
	d->kamoso = kamoso;
	d->collectionShared = new KamosoImageCollectionShared(kamoso);
// 	d->pluginLoader = new KIPI::PluginLoader(QStringList(), this);
}

KIPIInterface::~KIPIInterface() {
	delete d;
}

KIPI::ImageCollection KIPIInterface::currentAlbum() {
	return KIPI::ImageCollection(d->collectionShared);
}

KIPI::ImageCollection KIPIInterface::currentSelection() {
	return KIPI::ImageCollection(d->collectionShared);
}

QList<KIPI::ImageCollection> KIPIInterface::allAlbums() {
	QList<KIPI::ImageCollection> list;
	list << currentSelection();
	return list;
}

KIPI::ImageInfo KIPIInterface::info(const KUrl& url) {
	return KIPI::ImageInfo(new InfoShared(this,url));
}

bool KIPIInterface::addImage(const KUrl&, QString& err) 
{
	return true;
}
void KIPIInterface::delImage( const KUrl& ) 
{
	
}
void KIPIInterface::refreshImages( const KUrl::List& urls ) 
{
	
}

KIPI::ImageCollectionSelector* KIPIInterface::imageCollectionSelector(QWidget *parent) {
	return new KIPI::ImageCollectionSelector(parent);
}

KIPI::UploadWidget* KIPIInterface::uploadWidget(QWidget *parent) {
	return (new KIPI::UploadWidget(parent));
}

int KIPIInterface::features() const {
	return KIPI::HostAcceptNewImages;
}