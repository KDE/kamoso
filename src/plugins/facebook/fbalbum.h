/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-12-26
 * Description : a kipi plugin to import/export images to Facebook web service
 *
 * Copyright (C) 2008-2009 by Luka Renko <lure at kubuntu dot org>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef FBALBUM_H
#define FBALBUM_H

// Qt includes

// KDE includes
#include <KDialog>

class KLineEdit;
class KTextEdit;
class KComboBox;

namespace KIPIFacebookPlugin
{

class FbAlbum;

class FbNewAlbum : public KDialog
{
    Q_OBJECT

public:
    FbNewAlbum(QWidget* parent);
    ~FbNewAlbum();

    void getAlbumProperties(FbAlbum &album);

private:
    KLineEdit       *m_titleEdt;
    KLineEdit       *m_locEdt;
    KTextEdit       *m_descEdt;
    KComboBox       *m_privacyCoB;

    friend class FbWindow;
};

} // namespace KIPIFacebookPlugin

#endif // FBALBUM_H
