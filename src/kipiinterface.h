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

#ifndef KIPIINTERFACE_H
#define KIPIINTERFACE_H

#include <QList>
#include <libkipi/interface.h>
#include <libkipi/uploadwidget.h>
#include <libkipi/imagecollectionshared.h>
#include "kamoso.h"

class KIPIInterfacePrivate;
class KIPIInterface :public KIPI::Interface {
	Q_OBJECT
	
public:
	KIPIInterface(Kamoso* kamoso);
	virtual ~KIPIInterface();
	
	virtual bool addImage(const KUrl&, QString& err);
	virtual void delImage( const KUrl& );
	virtual void refreshImages( const KUrl::List& urls );

	virtual KIPI::ImageCollectionSelector* imageCollectionSelector(QWidget *parent);
	virtual KIPI::UploadWidget* uploadWidget(QWidget *parent);
	
	
	virtual QList< KIPI::ImageCollection > allAlbums();
	virtual KIPI::ImageCollection currentAlbum();
	virtual KIPI::ImageCollection currentSelection();
	virtual int features() const;
    virtual KIPI::ImageInfo info(const KUrl& );

private:
	private:
	KIPIInterfacePrivate* const d;
};

#endif // KIPIINTERFACE_H
