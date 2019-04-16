/*
    Copyright (C) 2011-2013 Collabora Ltd.
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GSTPOINTER_H
#define GSTPOINTER_H

#include <QScopedPointer>

template <typename T>
struct GstPointerCleanup
{
    static inline void cleanup(T *pointer)
    {
        if (pointer) gst_object_unref (pointer);
    }
};

template <typename T>
using GstPointer = QScopedPointer<T, GstPointerCleanup<T>>;

#endif
