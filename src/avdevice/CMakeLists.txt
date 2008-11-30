
include_directories( 
${KOPETE_INCLUDES} 
)


########### next target ###############

set(kopete_videodevice_LIB_SRCS 
   videodevice.cpp 
   videoinput.cpp 
   videodevicepool.cpp 
   videodevicemodelpool.cpp
   bayer.cpp
   sonix_compress.cpp )

kde4_add_library(kopete_videodevice SHARED ${kopete_videodevice_LIB_SRCS})

target_link_libraries(kopete_videodevice ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_SOLID_LIBS})

set_target_properties(kopete_videodevice PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kopete_videodevice ${INSTALL_TARGETS_DEFAULT_ARGS})


########### next target ###############


########### install files ###############





