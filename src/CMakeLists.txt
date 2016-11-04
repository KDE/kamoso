include_directories(${QTGSTREAMER_INCLUDE_DIR})

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)
add_definitions(-DQT_NO_KEYWORDS)

qt5_add_resources(kamosoqml_SRCS resources.qrc)
kconfig_add_kcfg_files(kamosoqml_SRCS kamosoSettings.kcfgc GENERATE_MOC)

add_executable(kamoso
    whitewidgetmanager.cpp 
    whitewidget.cpp
    kamosodirmodel.cpp
    main.cpp
    kamoso.cpp
    previewfetcher.cpp

    ${kamosoqml_SRCS}
)

target_link_libraries(kamoso
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick
    KF5::Declarative KF5::KIOWidgets KF5::KIOWidgets KF5::ConfigGui KF5::I18n
)

install(TARGETS kamoso ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kamoso.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
