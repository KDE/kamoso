set(facebook_SRCS
	facebook.cpp
	
	#from kipiplugins
	fbtalker.cpp mpform.cpp)

kde4_add_plugin(facebooksender ${facebook_SRCS})
target_link_libraries(facebooksender
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
			kamosoplugin)

install(TARGETS facebooksender DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES facebooksender.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kde4_add_unit_test(facebooktest facebooktest.cpp ${facebook_SRCS})
target_link_libraries(facebooktest
			${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}	
			${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTTEST_LIBRARY}
			kamosoplugin)
