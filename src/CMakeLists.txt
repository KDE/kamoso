
include_directories(${KDE4_INCLUDES})

add_subdirectory(avdevice)

set(kamoso_SRCS
	kamoso.cpp
	main.cpp
	whitewidget.cpp
	countdownwidget.cpp
	webcamwidget.cpp
	customDelegate.cpp
	webcamretriever.cpp
)

kde4_add_executable(kamoso ${kamoso_SRCS})
target_link_libraries(kamoso ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS} solidcontrol kopete_videodevice)
