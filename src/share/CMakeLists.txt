add_subdirectory(plugins)
add_subdirectory(tool)

add_library(KamosoShare
    shareinterface.cpp
    sharealternativesmodel.cpp
)

target_link_libraries(KamosoShare KF5::CoreAddons Qt5::Gui)
