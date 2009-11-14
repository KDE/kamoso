SET(PLUGIN_NAME kipiplugin_youtube)
add_subdirectory(icons)

set(${PLUGIN_NAME}_SRCS youtube.cpp youtubejob.cpp)

set(youtube_UI videoInfo.ui)
kde4_add_ui_files(${PLUGIN_NAME_SRCS} ${youtube_UI})

kde4_add_plugin(${PLUGIN_NAME} ${${PLUGIN_NAME}_SRCS})
target_link_libraries(${PLUGIN_NAME}
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} ${KIPI_LIBRARIES})

INSTALL(TARGETS ${PLUGIN_NAME} DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES ${PLUGIN_NAME}.desktop DESTINATION ${SERVICES_INSTALL_DIR})

