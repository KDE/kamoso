include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KIPI_INCLUDE_DIR}
	${QTGSTREAMER_INCLUDE_DIR} ${GLIB2_INCLUDE_DIR} ${GSTREAMER_INCLUDE_DIR})

qt4_add_resources(kamosoqml_SRCS resources.qrc)
kde4_add_kcfg_files(kamosoqml_SRCS settings.kcfgc)

kde4_add_executable(qmloso kamososettings.cpp kamosodirmodel.cpp kamosoquick.cpp main.cpp ${kamosoqml_SRCS})
target_link_libraries(qmloso ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTGUI_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} kdeclarative ${KDE4_KIO_LIBS})
