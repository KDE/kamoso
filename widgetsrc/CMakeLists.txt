include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${KIPI_INCLUDE_DIR} ${QTGSTREAMER_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR} ${GSTREAMER_INCLUDE_DIR})

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)

if (NEPOMUK_FOUND)
	include_directories(${NEPOMUK_INCLUDES})
endif()

add_subdirectory(plugins)
add_subdirectory(pluginTester)

set(kamoso_SRCS
	burstshootmode.cpp
	videoshootmode.cpp
	photoshootmode.cpp
	shootmode.cpp 
	thumbnailview.cpp
	kamoso.cpp
	main.cpp
	../src/whitewidget.cpp
	../src/whitewidgetmanager.cpp
	countdownwidget.cpp
	webcamwidget.cpp
	customDelegate.cpp
	../src/device.cpp
	../src/devicemanager.cpp
	kamosojobtracker.cpp 
	kipiinterface.cpp
	kamosoimagecollectionshared.cpp
	infoshared.cpp
	kipiaction.cpp
	webcamdialog.cpp
	pagewebcamconfigmanager.cpp
)
qt5_add_dbus_interface(kamoso_SRCS
    org.kde.Solid.PowerManagement.xml
    brightness_interface)

set(kamoso_UI mainWidget.ui generalConfig.ui pictureConfig.ui videoConfig.ui)

kconfig_add_kcfg_files(kamoso_SRCS ../src/settings.kcfgc)
qt5_wrap_ui(kamoso_SRCS ${kamoso_UI})
add_executable(kamoso ${kamoso_SRCS})

configure_file(config-nepomuk.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nepomuk.h)

if(${KDE_VERSION} VERSION_LESS 4.6.0)
          set (SolidControl solidcontrol)
endif()

target_link_libraries(kamoso ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} KF5::KIOWidgets
        KF5::Solid ${KDE4_PHONON_LIBS} ${KDE4_KUTILS_LIBS} ${KIPI_LIBRARIES} ${SolidControl} ${QTGSTREAMER_LIBRARY} ${QTGSTREAMER_UI_LIBRARY} ${GSTREAMER_BASE_LIBRARY})

if(HAVE_NEPOMUK)
	target_link_libraries(kamoso ${NEPOMUK_LIBRARIES})
endif()

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install(FILES kamosoplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
