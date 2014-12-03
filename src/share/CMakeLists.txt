add_subdirectory(plugins)

add_library(KamosoShare
    shareinterface.cpp
    sharealternativesmodel.cpp
)

target_link_libraries(KamosoShare KF5::CoreAddons Qt5::Gui)
