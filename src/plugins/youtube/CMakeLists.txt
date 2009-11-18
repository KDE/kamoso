add_subdirectory(icons)

set(youtube_SRCS youtubemanager.cpp kipiplugin_youtube.cpp youtubejob.cpp youtubejobcomposite.cpp)

set(youtube_UI videoInfo.ui)
kde4_add_ui_files(youtube_SRCS ${youtube_UI})

kde4_add_plugin(kipiplugin_youtube ${youtube_SRCS})
target_link_libraries(kipiplugin_youtube
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} ${KIPI_LIBRARIES})

install(TARGETS kipiplugin_youtube DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kipiplugin_youtube.desktop DESTINATION ${SERVICES_INSTALL_DIR})

