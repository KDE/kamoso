add_subdirectory(icons)

set(youtube_SRCS youtubemanager.cpp kipiplugin_youtube.cpp youtubejob.cpp youtubejobcomposite.cpp)

set(youtube_UI videoInfo.ui)
qt5_wrap_ui(youtube_SRCS ${youtube_UI})

add_library(kipiplugin_youtube MODULE ${youtube_SRCS})
target_link_libraries(kipiplugin_youtube
			 KF5::KIOWidgets	
			Qt5::Core Qt5::Gui Qt5::Xml ${KIPI_LIBRARIES})

install(TARGETS kipiplugin_youtube DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kipiplugin_youtube.desktop DESTINATION ${SERVICES_INSTALL_DIR})

