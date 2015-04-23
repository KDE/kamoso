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

#include <QtCore/QObject>

#include <solid/device.h>
#include <KSharedConfig>

namespace UdevQt {
    class Device;
}
class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(int gamma READ gamma WRITE setGamma NOTIFY gammaChanged)

    public:
        Device(const UdevQt::Device &device, QObject* parent = 0);
        ~Device();
        QString description() const;
        QString udi() const;
        QByteArray path() const;
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

    Q_SIGNALS:
        void brightnessChanged(int value);
        void hueChanged(int value);
        void contrastChanged(int value);
        void saturationChanged(int value);
        void gammaChanged(int value);

    private:
        const QString m_description;
        const QString m_udi;
        const QByteArray m_path;
        const QString m_vendor;
        const KSharedConfigPtr m_config;
};

#endif //DEVICE_H
