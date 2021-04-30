/*
    SPDX-FileCopyrightText: 2011-2013 Collabora Ltd.
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

    SPDX-License-Identifier: LGPL-2.1-or-later
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
