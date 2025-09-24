add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)
add_definitions(-DQT_NO_KEYWORDS)

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KAMOSO VERSION_HEADER kamoso_version.h)

add_executable(kamoso
    devicemanager.cpp
    device.cpp
    kamosodirmodel.cpp
    main.cpp
    kamoso.cpp
    previewfetcher.cpp
    video/webcamcontrol.cpp

    resources.qrc
)

kconfig_add_kcfg_files(kamoso kamosoSettings.kcfgc GENERATE_MOC)
target_link_libraries(kamoso PRIVATE
    Qt::Core Qt::Gui Qt::Widgets Qt::Quick
    KF6::KIOFileWidgets KF6::ConfigGui KF6::I18n KF6::Notifications
    PkgConfig::GStreamerVideo PkgConfig::GStreamer
    KirigamiApp
)

install(TARGETS kamoso ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kamoso.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES kamoso.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
