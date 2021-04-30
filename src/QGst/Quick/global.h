/*
    SPDX-FileCopyrightText: 2011-2013 Collabora Ltd.
    SPDX-FileCopyrightText: George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef QTGSTREAMERQUICK_GLOBAL_H
#define QTGSTREAMERQUICK_GLOBAL_H

// workaround for https://bugreports.qt-project.org/browse/QTBUG-22829
#if defined(Q_MOC_RUN) && !defined(BOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
#define BOOST_TT_HAS_OPERATOR_HPP_INCLUDED
#endif

#include <QtGlobal>

/* defined by cmake when building this library */
#if defined(QtGStreamerQuick_EXPORTS) || defined(Qt5GStreamerQuick_EXPORTS)
# define QTGSTREAMERQUICK_EXPORT Q_DECL_EXPORT
#else
# define QTGSTREAMERQUICK_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define QTGSTREAMERQUICK_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define QTGSTREAMERQUICK_NO_EXPORT
#endif

#endif
