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

#include "pluginTester.h"

#include <stdio.h>
#include <signal.h>

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>

//This is ugly, I know, but the class is not being deleted when executing qApp->quit :/
static PluginTester *pluginTester;
void signalHandler(int signal)
{
    Q_UNUSED(signal)

    delete pluginTester;
    qApp->quit();
}

int main(int argc, char *argv[])
{
    KAboutData about("kamosoPluginTester", 0, ki18n(("Kamoso Plugin Tester")), "1.90", ki18n("Plugin tester"),
             KAboutData::License_GPL, ki18n("(C) 2008-2010 Alex Fiestas and Aleix Pol"));
    about.addAuthor( ki18n("Aleix Pol Gonzalez"), ki18n("Semaphore hacker"), "aleixpol@kde.org" );
    about.addAuthor( ki18n("Alex Fiestas"), ki18n("Coffee drinker"), "alex@eyeos.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+<args>", ki18n("asdasd"));
    KCmdLineArgs::addCmdLineOptions(options);

    if (KCmdLineArgs::parsedArgs()->count() < 2) {
        kDebug() << "Usage: \n";
        kDebug() << "kamosoPluginTester pluginName file1 file2 file3...\n";
        return -1;
    }
    KApplication app;

    app.setQuitOnLastWindowClosed(false);
    pluginTester = new PluginTester;

    return app.exec();
}
