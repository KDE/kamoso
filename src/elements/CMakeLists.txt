set(GST_PACKAGE_NAME "KamosoGStreamer" CACHE STRING "The name of this package")
set(GST_PACKAGE_ORIGIN "https://gstreamer.freedesktop.org/" CACHE STRING "The origin url of this package")
set(QTGSTREAMER_VERSION 1.2.0)
set(QTVIDEOSINK_NAME kamosoqt5videosink)

add_definitions(-DPACKAGE="kamoso-gstreamer"
                -DPACKAGE_NAME="${GST_PACKAGE_NAME}"
                -DPACKAGE_ORIGIN="${GST_PACKAGE_ORIGIN}"
                -DPACKAGE_VERSION="${QTGSTREAMER_VERSION}"
                -DGST_DISABLE_XML
                -DGST_DISABLE_LOADSAVE)

add_subdirectory(gstqtvideosink)
