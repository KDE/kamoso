add_subdirectory(elements)

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)
add_definitions(-DQT_NO_KEYWORDS)

qt_add_resources(kamosoqml_SRCS resources.qrc)
kconfig_add_kcfg_files(kamosoqml_SRCS kamosoSettings.kcfgc GENERATE_MOC)

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KAMOSO VERSION_HEADER kamoso_version.h)

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

    QGst/Quick/videosurface.cpp
    QGst/Quick/videoitem.cpp

    ${kamosoqml_SRCS}
)

target_link_libraries(kamoso
    Qt::Core Qt::Gui Qt::Widgets Qt::Quick
    KF6::KIOFileWidgets KF6::ConfigGui KF6::I18n KF6::Notifications
    PkgConfig::GStreamer
)

install(TARGETS kamoso ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kamoso.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES kamoso.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
