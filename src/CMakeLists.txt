
include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(avdevice)
add_subdirectory(plugins)

set(kamoso_SRCS 
	thumbnailview.cpp
	kamoso.cpp
	main.cpp
	whitewidget.cpp
	whitewidgetmanager.cpp
	countdownwidget.cpp
	webcamwidget.cpp
	customDelegate.cpp
	webcamretriever.cpp
	timedpushbutton.cpp
	pluginmanager.cpp
	device.cpp
	devicemanager.cpp
)

set(kamoso_UI mainWidget.ui generalConfig.ui pictureConfig.ui)

kde4_add_library(kamosoplugin SHARED kamosoplugin.cpp)
target_link_libraries(kamosoplugin ${KDE4_KDEUI_LIBS})

kde4_add_kcfg_files(kamoso_SRCS settings.kcfgc)
kde4_add_ui_files(kamoso_SRCS ${kamoso_UI})
kde4_add_executable(kamoso ${kamoso_SRCS})
target_link_libraries(kamoso kamosoplugin ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS}
		${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS} ${KDE4_KUTILS_LIBS} solidcontrol kamoso_videodevice)

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS kamosoplugin ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install(FILES kamosoplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

