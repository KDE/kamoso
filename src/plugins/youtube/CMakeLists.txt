set(youtube_SRCS youtube.cpp youtubemanager.cpp)
	

set(youtube_UI auth.ui)
kde4_add_ui_files(youtube_SRCS ${youtube_UI})

kde4_add_plugin(youtube ${youtube_SRCS})
target_link_libraries(youtube
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
			kamosoplugin)

install(TARGETS youtube DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES youtube.desktop DESTINATION ${SERVICES_INSTALL_DIR})

