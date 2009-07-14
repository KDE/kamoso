/*************************************************************************************
 *  Copyright (C) 2008 by Aleix Pol <aleixpol@gmail.com>                             *
 *  Copyright (C) 2008 by Alex Fiestas <alex@eyeos.org>                              *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 3                   *
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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include "kamoso.h"

int main(int argc, char *argv[])
{
	KAboutData about("kamoso", 0, ki18n(("Kamoso")), "0.8", ki18n("A sweet girl attractor"),
			 KAboutData::License_GPL, ki18n("(C) 2008-2009 Alex Fiestas and Aleix Pol"));
	about.addAuthor( ki18n("Aleix Pol Gonzalez"), KLocalizedString(), "aleixpol@gmail.com" );
	about.addAuthor( ki18n("Alex Fiestas"), KLocalizedString(), "alex@eyeos.org" );
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	
	Kamoso* kamosoMain = new Kamoso;
	kamosoMain->show();
	
	return app.exec();
}
