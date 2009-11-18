
include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${VLC_INCLUDE_DIRS} /usr/include/vlc/plugins)

if (NEPOMUK_FOUND)
	include_directories(${NEPOMUK_INCLUDES})
endif(NEPOMUK_FOUND)

add_subdirectory(plugins)

set(kamoso_SRCS
	burstshootmode.cpp
	videoshootmode.cpp
	photoshootmode.cpp
	shootmode.cpp 
	thumbnailview.cpp
	kamoso.cpp
	main.cpp
	whitewidget.cpp
	whitewidgetmanager.cpp
	countdownwidget.cpp
	webcamwidget.cpp
	customDelegate.cpp
	timedpushbutton.cpp
	pluginmanager.cpp
	device.cpp
	devicemanager.cpp
	kamosojobtracker.cpp
	webcamdialog.cpp
	pagewebcamconfigmanager.cpp
)

set(kamoso_UI mainWidget.ui generalConfig.ui pictureConfig.ui videoConfig.ui)

kde4_add_library(kamosoplugin SHARED kamosojob.cpp kamosoplugin.cpp)
target_link_libraries(kamosoplugin ${KDE4_KDEUI_LIBS})

kde4_add_kcfg_files(kamoso_SRCS settings.kcfgc)
kde4_add_ui_files(kamoso_SRCS ${kamoso_UI})
kde4_add_executable(kamoso ${kamoso_SRCS})

configure_file(config-nepomuk.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nepomuk.h)
target_link_libraries(kamoso kamosoplugin ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS}
		${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS} ${KDE4_KUTILS_LIBS} ${VLC_LIBRARIES} solidcontrol)

if(HAVE_NEPOMUK)
	target_link_libraries(kamoso ${NEPOMUK_LIBRARIES})
endif(HAVE_NEPOMUK)

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS kamosoplugin ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install(FILES kamosoplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

