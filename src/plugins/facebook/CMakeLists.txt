set(facebook_SRCS facebook.cpp)

kde4_add_plugin(facebooksender ${facebook_SRCS})
target_link_libraries(facebooksender ${KDE4_KDECORE_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} kamosoplugin)

install(TARGETS facebooksender DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES facebooksender.desktop DESTINATION ${SERVICES_INSTALL_DIR})
