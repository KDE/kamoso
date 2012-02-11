project(kamoso)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(KDE4 REQUIRED)
find_package(Kipi REQUIRED)
find_package(Phonon REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(GStreamer REQUIRED)
find_package(QtGStreamer REQUIRED)

macro_optional_find_package(Nepomuk)
macro_log_feature(NEPOMUK_FOUND "Nepomuk" "Handles all kinds of metadata on the KDE desktop to provide a semantic desktop" "http://nepomuk.kde.org/" FALSE "" "STRONGLY_RECOMMENDED: Nepomuk is used to provide metadata on objects like files, emails etc. to provide a semantic desktop.")
macro_bool_to_01(NEPOMUK_FOUND HAVE_NEPOMUK)
macro_display_feature_log()
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(widgetsrc)
add_subdirectory(icons)
