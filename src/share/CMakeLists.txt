add_library(Purpose
    pluginbase.cpp
    job.cpp
    alternativesmodel.cpp
)
target_include_directories(Purpose PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

target_link_libraries(Purpose KF5::CoreAddons Qt5::Gui)

add_subdirectory(plugins)
add_subdirectory(tool)
add_subdirectory(autotests)

kcoreaddons_desktop_to_json(Purpose plugins/ExportPluginType.desktop)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ExportPluginType.json DESTINATION ${DATA_INSTALL_DIR}/purpose/types)
