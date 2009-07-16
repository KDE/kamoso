
# install(FILES settings.kcfg DESTINATION ${KCFG_INSTALL_DIR})

include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(avdevice)

set(kamoso_SRCS
	thumbnailview.cpp
	kamoso.cpp
	main.cpp
	whitewidget.cpp
	whitewidgetmanager.cpp
	countdownwidget.cpp
	webcamwidget.cpp
	customDelegate.cpp
	webcamretriever.cpp
	timedpushbutton.cpp
)
kde4_add_kcfg_files(kamoso_SRCS settings.kcfgc)
kde4_add_ui_files(kamoso_SRCS mainWidget.ui)
kde4_add_ui_files(kamoso_SRCS generalConfig.ui)
kde4_add_ui_files(kamoso_SRCS pictureConfig.ui)
kde4_add_executable(kamoso ${kamoso_SRCS})
target_link_libraries(kamoso ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS} solidcontrol kopete_videodevice)
