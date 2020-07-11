add_subdirectory(elements)

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)
add_definitions(-DQT_NO_KEYWORDS)

qt5_add_resources(kamosoqml_SRCS resources.qrc)
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

target_include_directories(kamoso PRIVATE "${GSTREAMER_INCLUDE_DIR}" "${GLIB2_INCLUDE_DIR}")

target_link_libraries(kamoso
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick
    KF5::KIOFileWidgets KF5::ConfigGui KF5::I18n KF5::Notifications
    ${GSTREAMER_LIBRARIES} ${GLIB2_LIBRARIES} ${GOBJECT_LIBRARIES}
)

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kamoso.notifyrc DESTINATION ${KNOTIFYRC_INSTALL_DIR})
install(FILES kamoso-shutter.wav DESTINATION ${SOUND_INSTALL_DIR})
