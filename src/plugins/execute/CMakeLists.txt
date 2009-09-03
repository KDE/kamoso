set(execute_SRCS execute.cpp)
	

kde4_add_plugin(execute ${execute_SRCS})
target_link_libraries(execute
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
			kamosoplugin)

install(TARGETS execute DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES execute.desktop DESTINATION ${SERVICES_INSTALL_DIR})

