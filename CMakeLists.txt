project(kamoso)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(KDE4 REQUIRED)
find_package(LibV4L2 REQUIRED)
find_package(VLC REQUIRED)
find_package(Kipi REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(po)
