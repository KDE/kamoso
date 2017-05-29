include_directories(${QTGSTREAMER_INCLUDE_DIR})

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)
add_definitions(-DQT_NO_KEYWORDS)

qt5_add_resources(kamosoqml_SRCS resources.qrc)
kconfig_add_kcfg_files(kamosoqml_SRCS kamosoSettings.kcfgc GENERATE_MOC)

add_executable(kamoso
    devicemanager.cpp
    device.cpp
    whitewidgetmanager.cpp 
    whitewidget.cpp
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
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick
    KF5::KIOFileWidgets KF5::ConfigGui KF5::I18n
    Qt5GStreamer Qt5GStreamerQuick
    ${UDEV_LIBS}
)

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
