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

#include "../shareinterface.h"
#include <QDebug>
#include <QTimer>
#include <KPluginFactory>

class DummyShareJob : public ShareJob
{
    Q_OBJECT
    public:
        virtual void start() override
        {
            qWarning() << "xxxxxxxx";
            QTimer::singleShot(0, this, [this](){ emitResult(); });
        }
};

class Q_DECL_EXPORT DummyPlugin : public SharePlugin
{
    Q_OBJECT
    public:
        DummyPlugin(QObject*, const QVariantList& ) {}

        virtual ShareJob* share(const QMimeData& data) const override
        {
            return new DummyShareJob;
        }
};

K_PLUGIN_FACTORY_WITH_JSON(DummyShare, "dummyplugin.json", registerPlugin<DummyPlugin>();)

#include "dummyplugin.moc"
