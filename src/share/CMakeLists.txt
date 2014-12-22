add_library(KamosoShare
    shareinterface.cpp
    sharealternativesmodel.cpp
)
target_include_directories(KamosoShare PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_link_libraries(KamosoShare KF5::CoreAddons Qt5::Gui)

add_subdirectory(plugins)
add_subdirectory(tool)

kcoreaddons_desktop_to_json(KamosoShare plugins/ExportPluginType.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ExportPluginType.json DESTINATION ${DATA_INSTALL_DIR}/kamoso/types)
