include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KIPI_INCLUDE_DIR}
	${QTGSTREAMER_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR} ${GSTREAMER_INCLUDE_DIR})

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)
add_definitions(-DQT_NO_KEYWORDS)

qt5_add_resources(kamosoqml_SRCS resources.qrc)
kconfig_add_kcfg_files(kamosoqml_SRCS settings.kcfgc)

add_executable(kamoso
    devicemanager.cpp
    device.cpp
    whitewidgetmanager.cpp 
    whitewidget.cpp
    kamososettings.cpp
    kamosodirmodel.cpp
    main.cpp
    kamoso.cpp
    previewfetcher.cpp
    video/webcamcontrol.cpp
    udev/udevqtclient.cpp
    udev/udevqtdevice.cpp

    ${kamosoqml_SRCS}
)

target_link_libraries(kamoso
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::OpenGL
    KF5::Declarative KF5::KIOWidgets KF5::Solid KF5::KIOWidgets KF5::ConfigGui KF5::I18n
    Qt5GStreamer Qt5GStreamer Qt5GStreamerQuick
    ${GSTREAMER_BASE_LIBRARY} ${GSTREAMER_LIBRARY} ${GLIB2_LIBRARIES} ${UDEV_LIBS}
)

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
