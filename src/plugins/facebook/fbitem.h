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

#ifndef FBITEM_H
#define FBITEM_H

// Qt includes
#include <QString>

namespace KIPIFacebookPlugin
{

class FbUser
{
public:
    FbUser()
    {
        id = 0;
        uploadPerm = false;
    }

    void clear()
    {
        id = 0;
        name.clear();
        profileURL.clear();
        uploadPerm = false;
    }

    long long   id;

    QString     name;
    QString     profileURL;
    bool        uploadPerm;
};

enum FbPrivacy
{
    FB_FRIENDS = 0,
    FB_FRIENDS_OF_FRIENDS,
    FB_NETWORKS,
    FB_EVERYONE
};

class FbAlbum
{
public:
    FbAlbum()
    {
        id = -1;
        privacy = FB_FRIENDS;
    }

    long long   id;

    QString     title;
    QString     description;
    QString     location;
    FbPrivacy   privacy;
    QString     url;
};

class FbPhoto
{
public:
    FbPhoto()
    {
        id = -1;
    }

    long long   id;

    QString     caption;
    QString     thumbURL;
    QString     originalURL;
};

} // namespace KIPIFacebookPlugin

#endif /* FBITEM_H */
