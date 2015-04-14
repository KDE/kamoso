# - Try to find GStreamer
# Once done this will define
#
#  GSTREAMER_FOUND - system has GStreamer
#  GSTREAMER_INCLUDE_DIR - the GStreamer include directory
#  GSTREAMER_LIBRARY - the main GStreamer library
#  GSTREAMER_PLUGIN_DIR - the GStreamer plugin directory
#
#  And for all the plugin libraries specified in the COMPONENTS
#  of find_package, this module will define:
#
#  GSTREAMER_<plugin_lib>_LIBRARY_FOUND - system has <plugin_lib>
#  GSTREAMER_<plugin_lib>_LIBRARY - the <plugin_lib> library
#  GSTREAMER_<plugin_lib>_INCLUDE_DIR - the <plugin_lib> include directory
#
# Copyright (c) 2010, Collabora Ltd.
#   @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# TODO: Other versions --> GSTREAMER_X_Y_FOUND (Example: GSTREAMER_0_8_FOUND and GSTREAMER_0_10_FOUND etc)

IF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_BASE_LIBRARY)
   # in cache already
   SET(GStreamer_FIND_QUIETLY TRUE)
ELSE (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_BASE_LIBRARY)
   SET(GStreamer_FIND_QUIETLY FALSE)
ENDIF (GSTREAMER_INCLUDE_DIR AND GSTREAMER_LIBRARIES AND GSTREAMER_BASE_LIBRARY)

SET(GSTREAMER_API_VERSION 1.0)
IF (NOT WIN32)
   FIND_PACKAGE(PkgConfig REQUIRED)
   # use pkg-config to get the directories and then use these values
   # in the FIND_PATH() and FIND_LIBRARY() calls
   # don't make this check required - otherwise you can't use macro_optional_find_package on this one
   PKG_CHECK_MODULES(PKG_GSTREAMER gstreamer-${GSTREAMER_API_VERSION})
   SET(GSTREAMER_VERSION ${PKG_GSTREAMER_VERSION})
   SET(GSTREAMER_DEFINITIONS ${PKG_GSTREAMER_CFLAGS})
ENDIF (NOT WIN32)

MESSAGE(STATUS "Found GStreamer package: ${PKG_GSTREAMER_VERSION}")

FIND_PATH(GSTREAMER_INCLUDE_DIR gst/gst.h
   PATHS
   ${PKG_GSTREAMER_INCLUDE_DIRS}
   PATH_SUFFIXES gstreamer-${GSTREAMER_API_VERSION}
   )

FIND_LIBRARY(GSTREAMER_LIBRARIES NAMES gstreamer-${GSTREAMER_API_VERSION}
   PATHS
   ${PKG_GSTREAMER_LIBRARY_DIRS}
   )

FIND_LIBRARY(GSTREAMER_BASE_LIBRARY NAMES gstbase-${GSTREAMER_API_VERSION}
   PATHS
   ${PKG_GSTREAMER_LIBRARY_DIRS}
   )

IF (GSTREAMER_INCLUDE_DIR)
ELSE (GSTREAMER_INCLUDE_DIR)
   MESSAGE(STATUS "GStreamer: WARNING: include dir not found")
ENDIF (GSTREAMER_INCLUDE_DIR)

IF (GSTREAMER_LIBRARIES)
ELSE (GSTREAMER_LIBRARIES)
   MESSAGE(STATUS "GStreamer: WARNING: library not found")
ENDIF (GSTREAMER_LIBRARIES)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GStreamer  DEFAULT_MSG  GSTREAMER_LIBRARIES GSTREAMER_INCLUDE_DIR GSTREAMER_BASE_LIBRARY)

MARK_AS_ADVANCED(GSTREAMER_INCLUDE_DIR GSTREAMER_LIBRARIES GSTREAMER_BASE_LIBRARY)
