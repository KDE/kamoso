
if (LIBV4L2_FOUND)
include_directories( 
${KOPETE_INCLUDES} 
${LIBV4L2_INCLUDE_DIR}
)
else (LIBV4L2_FOUND)
include_directories(
${KOPETE_INCLUDES}
)
endif (LIBV4L2_FOUND)

########### next target ###############
set(KAMOSO_VIDEODEVICE_LIB_VERSION 1.0.0)
set(KAMOSO_VIDEODEVICE_LIB_SOVERSION 1)

set(kopete_videodevice_LIB_SRCS 
   videodevice.cpp 
   videoinput.cpp 
   dummyvideodevice.cpp
   videodevicevector.cpp
   videodevicepool.cpp 
   videodevicemodelpool.cpp
   bayer.cpp
   sonix_compress.cpp )

kde4_add_library(kamoso_videodevice SHARED ${kopete_videodevice_LIB_SRCS})

if (LIBV4L2_FOUND)
target_link_libraries(kamoso_videodevice${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_SOLID_LIBS} ${LIBV4L2_LIBRARY})
else (LIBV4L2_FOUND)
target_link_libraries(kamoso_videodevice ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_SOLID_LIBS})
endif (LIBV4L2_FOUND)

set_target_properties(kamoso_videodevice PROPERTIES VERSION ${KAMOSO_VIDEODEVICE_LIB_VERSION} SOVERSION ${KAMOSO_VIDEODEVICE_LIB_SOVERSION})
install(TARGETS kamoso_videodevice ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############


########### install files ###############





