set(GST_PACKAGE_NAME "KamosoGStreamer" CACHE STRING "The name of this package")
set(GST_PACKAGE_ORIGIN "http://gstreamer.freedesktop.org/" CACHE STRING "The origin url of this package")
set(QTGSTREAMER_VERSION 1.2.0)
set(QTVIDEOSINK_NAME kamoso-qt5videosink)

add_definitions(-DPACKAGE="kamoso-gstreamer"
                -DPACKAGE_NAME="${GST_PACKAGE_NAME}"
                -DPACKAGE_ORIGIN="${GST_PACKAGE_ORIGIN}"
                -DPACKAGE_VERSION="${QTGSTREAMER_VERSION}"
                -DGST_DISABLE_XML
                -DGST_DISABLE_LOADSAVE)

include_directories(${GSTREAMER_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR})

include_directories(${GSTREAMER_BASE_INCLUDE_DIR} ${GSTREAMER_VIDEO_INCLUDE_DIR})
add_subdirectory(gstqtvideosink)
