/*************************************************************************************
 *  Copyright (C) 2008-2011 by Aleix Pol <aleixpol@kde.org>                          *
 *  Copyright (C) 2008-2011 by Alex Fiestas <alex@eyeos.org>                         *
 *  Copyright (C) 2013      by Michael G. Hansen <mike@mghansen.de>                  *
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

#ifndef KAMOSO_H
#define KAMOSO_H

#include <KMainWindow>
#include <KUrl>
#include <KFileItemList>
#include <KAction>
#include <libkipi/pluginloader.h>

class KDirModel;
class KamosoJobTracker;
class QToolButton;
class QRadioButton;
class ShootMode;
class WhiteWidgetManager;
class WebcamWidget;
class WebcamRetriever;
class CountdownWidget;
class QItemSelection;
class QStackedLayout;
class QSplitter;
class QPushButton;
class QToolButton;
class QModelIndex;
class ThumbnailView;
class KFileItem;
class WebcamWidget;
class DeviceManager;
class KamosoJob;
class WebcamDialog;
namespace Ui { class mainWidget; class webcamConfigWidget;}
namespace Phonon { class MediaObject; }
class Kamoso : public KMainWindow
{
Q_OBJECT
    public:
        Kamoso ( QWidget *parent=0 );
        void checkInitConfig();
        CountdownWidget *countdown() const;
        ~Kamoso();
        bool isFlashEnabled() const { return m_flashEnabled; }

        void startVideo(bool withSound);
        void stopVideo();
        KUrl::List selectedItems();
        KamosoJobTracker* tracker() const { return mTracker; }
        virtual void contextMenuEvent(QContextMenuEvent* event);

    public Q_SLOTS:
        void takePhoto();

        /** Starts a countdown before we take a picture.
            @param minimumTime in miliseconds */
        void startCountdown(qreal minimumTime=0);
        void configuration();
        void generalUpdated();
        void webcamChanged(int index);
        void webcamAdded();
        void webcamRemoved();
        void thumbnailAdded();
        void selectLast();
        void settingsMenu(bool);
        void selectJob(KJob* job, const KUrl::List& urls);
        void changeMode(bool);

        void setFlashEnabled(bool en) { m_flashEnabled=en; }
        void stopCountdown();
        void thumbnailViewMoved(int value);
        void updateThumbnails(const KUrl::List& urls);
        void exportMenu(bool);
        void fileViewSelectionChanged(const QItemSelection&, const QItemSelection&);

    private Q_SLOTS:
        void initialize();

        void restore();
        void slotScrollLeft();
        void slotScrollRight();
        void refreshDeviceList();
        void autoincFilename(KUrl& filename);
        void removeSelection();
        void openFile();

    Q_SIGNALS:
        void webcamPlaying(const QString&);

    private:
        QPointer<QMenu> exportKIPIMenu();

        #if KDE_IS_VERSION(4,5,85)
            int brightBack;
        #else
            float brightBack;
        #endif

        int m_webcamId;
        KDirModel *dirModel;
        WhiteWidgetManager *whiteWidgetManager;
        WebcamWidget *m_webcam;
        CountdownWidget *m_countdown;
        Phonon::MediaObject *player;
        Ui::mainWidget *mainWidgetUi;
        Ui::webcamConfigWidget *pageWebcam;
        QWidget *mainWidget;
        DeviceManager *deviceManager;
        int m_exponentialValue;
        QList<ShootMode*> m_modes;
        QList<QToolButton*> m_modesRadio;
        ShootMode *m_activeMode;
        bool m_flashEnabled;
        KIPI::PluginLoader* mPluginLoader;
        QList<KAction*> kipiActions;
        KamosoJobTracker* mTracker;
        WebcamDialog* dialog;
};

#endif
