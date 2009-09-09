set(trash_SRCS trash.cpp)
	

kde4_add_plugin(trash ${trash_SRCS})
target_link_libraries(trash
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
			kamosoplugin)

install(TARGETS trash DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES trash.desktop DESTINATION ${SERVICES_INSTALL_DIR})

