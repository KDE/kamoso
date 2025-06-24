cmake_minimum_required (VERSION 3.28 FATAL_ERROR)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "07")
set (RELEASE_SERVICE_VERSION_MICRO "70")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Kamoso VERSION ${RELEASE_SERVICE_VERSION})

set(KF6_MIN_VERSION 6.0.0)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)
include(ECMSetupVersion)

find_package(Qt6 NO_MODULE REQUIRED COMPONENTS Core Gui Widgets Quick)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Config DocTools KIO I18n Purpose Notifications)

option(PATCHED_GSTREAMER "GStreamer includes https://gitlab.freedesktop.org/gstreamer/gstreamer/-/merge_requests/9085" OFF)
if (NOT PATCHED_GSTREAMER)
    set(GSTREAMER_MIN_VERSION "1.26.3")
else()
    set(GSTREAMER_MIN_VERSION "1.22")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GStreamer IMPORTED_TARGET gstreamer-1.0>=${GSTREAMER_MIN_VERSION})
pkg_check_modules(GStreamerVideo IMPORTED_TARGET gstreamer-video-1.0)

include(ECMQMLModules)

ecm_find_qmlmodule(org.kde.purpose 1.0)
ecm_find_qmlmodule(org.kde.kirigami 1.0)

add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(doc)

ki18n_install(po)
kdoctools_install(po)

install(FILES org.kde.kamoso.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
