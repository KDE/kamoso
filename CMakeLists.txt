project(kamoso)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(KDE4 REQUIRED)
find_package(LibV4L2 REQUIRED)
include(MacroLogFeature)
include(MacroBoolTo01)

macro_optional_find_package(LibV4L2)
macro_bool_to_01(LIBV4L2_FOUND HAVE_LIBV4L2)
macro_log_feature(LIBV4L2_FOUND "libv4l2" "Collection of video4linux support libraries" "http://hansdegoede.livejournal.com/3636.html" FALSE "" "Required for better webcam support")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-kopete.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kopete.h )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

