
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

set(kopete_videodevice_LIB_SRCS 
   videodevice.cpp 
   videoinput.cpp 
   dummyvideodevice.cpp
   videodevicevector.cpp
   videodevicepool.cpp 
   videodevicemodelpool.cpp
   bayer.cpp
   sonix_compress.cpp )

kde4_add_library(kopete_videodevice SHARED ${kopete_videodevice_LIB_SRCS})

if (LIBV4L2_FOUND)
target_link_libraries(kopete_videodevice ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_SOLID_LIBS} ${LIBV4L2_LIBRARY})
else (LIBV4L2_FOUND)
target_link_libraries(kopete_videodevice ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_SOLID_LIBS})
endif (LIBV4L2_FOUND)

set_target_properties(kopete_videodevice PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kopete_videodevice ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############


########### install files ###############





