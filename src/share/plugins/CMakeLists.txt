function(add_share_plugin name)
    add_library(${name} MODULE ${ARGN})
    target_link_libraries(${name} Qt5::Core KamosoShare)

    kcoreaddons_desktop_to_json(${name} ${name}.desktop)
    install(TARGETS ${name} DESTINATION ${PLUGIN_INSTALL_DIR}/kamoso/share)
endfunction()

add_share_plugin(dummyplugin dummyplugin.cpp)
