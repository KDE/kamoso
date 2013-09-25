/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
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

#ifndef DEVICE_H
#define DEVICE_H

#include <solid/device.h>
#include <KConfig>

class Device
{
    public:
        Device();
        Device(const Solid::Device*);
        ~Device();
        QString description() const;
        QString udi() const;
        QString path() const;
        QString vendor() const;
        void setBrightness(int level);
        void setContrast(int level);
        void setSaturation(int level);
        void setGamma(int level);
        void setHue(int level);
        int brightness() const;
        int contrast() const;
        int saturation() const;
        int gamma() const;
        int hue() const;
    private:
        QString queryv4lInfo();
        QString m_description;
        QString m_udi;
        QString m_path;
        QString m_vendor;
        KConfig* config;
};

#endif //DEVICE_H
