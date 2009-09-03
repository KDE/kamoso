find_package(LibGCal)
message(STATUS "caca ${LIBGCAL_INCLUDE_DIRS}")
include_directories( ${LIBGCAL_INCLUDE_DIRS} )
set(youtube_SRCS youtube.cpp)
	

kde4_add_plugin(youtube ${youtube_SRCS})
target_link_libraries(youtube
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} ${LIBGCAL_LIBRARIES} gdata ${GLIB2_LIBRARIES}
			gobject-2.0 kamosoplugin glib-2.0 gio-2.0)

install(TARGETS youtube DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES youtube.desktop DESTINATION ${SERVICES_INSTALL_DIR})

