project(kamoso)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(KDE4 REQUIRED)
find_package(Kipi REQUIRED)
find_package(Phonon REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(GStreamer 1.0.0)
find_package(QtGStreamer 1.1.90 REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(doc)
