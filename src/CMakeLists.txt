include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KIPI_INCLUDE_DIR}
	${QTGSTREAMER_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR} ${GSTREAMER_INCLUDE_DIR})

add_definitions(-DGST_DISABLE_DEPRECATED)
add_definitions(-fexceptions)

qt5_add_resources(kamosoqml_SRCS resources.qrc)
kconfig_add_kcfg_files(kamosoqml_SRCS settings.kcfgc)

add_executable(qmloso
    device.cpp
    devicemanager.cpp
    whitewidgetmanager.cpp 
    whitewidget.cpp
    kamososettings.cpp
    kamosodirmodel.cpp
    main.cpp
    kamoso.cpp
    previewfetcher.cpp
    video/webcamcontrol.cpp
    ${kamosoqml_SRCS}
)

target_link_libraries(qmloso
Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::OpenGL
    KF5::Declarative KF5::KIOWidgets KF5::Solid KF5::KIOWidgets KF5::ConfigGui KF5::KDE4Support
    ${QTGSTREAMER_LIBRARY} ${QTGSTREAMER_UI_LIBRARY} ${GSTREAMER_BASE_LIBRARY} Qt5GStreamerQuick
)
