qt5_add_resources(sharetool_SRCS sharetool-resources.qrc)

add_executable(sharetool main.cpp ${sharetool_SRCS})
target_link_libraries(sharetool KamosoShare KF5::I18n KF5::Declarative Qt5::Qml Qt5::Widgets)
