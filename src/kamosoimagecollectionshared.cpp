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

#include "kamosoimagecollectionshared.h"
#include <KFileItemList>

struct KamosoImageCollectionShared::Private
{
	Kamoso* kamoso;
};

KamosoImageCollectionShared::KamosoImageCollectionShared(Kamoso* kamoso): d(new Private)
{
	d->kamoso = kamoso;
}


KamosoImageCollectionShared::~KamosoImageCollectionShared()
{
	delete d;
}


QString KamosoImageCollectionShared::name()
{
	//TODO: Do this configurable?
	return "Kamoso";
}


KUrl::List KamosoImageCollectionShared::images()
{
	KFileItemList fileList = d->kamoso->selectedItems();
	return fileList.urlList();
}

