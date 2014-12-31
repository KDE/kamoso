function(add_share_plugin name)
    add_library(${name} MODULE ${ARGN})
    target_link_libraries(${name} Qt5::Core Purpose)

    kcoreaddons_desktop_to_json(${name} ${name}.desktop)
    install(TARGETS ${name} DESTINATION ${PLUGIN_INSTALL_DIR}/kamoso/share)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}_config.qml")
        install(FILES "${name}_config.qml" DESTINATION ${DATA_INSTALL_DIR}/kamoso/share)
    endif()
endfunction()

add_subdirectory(dummy)
add_subdirectory(imgur)
add_subdirectory(youtube)
