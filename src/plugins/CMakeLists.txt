include_directories(${CMAKE_SOURCE_DIR}/src ${LIBGCAL_INCLUDE_DIRS}  ${GLIB2_INCLUDE_DIR} /usr/include/libgdata /usr/include/libxml2 /usr/include/libsoup-2.4)
add_subdirectory(facebook)
add_subdirectory(execute)
add_subdirectory(youtube)
