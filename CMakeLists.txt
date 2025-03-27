cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "03")
set (RELEASE_SERVICE_VERSION_MICRO "90")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(Kamoso VERSION ${RELEASE_SERVICE_VERSION})

set(KF5_MIN_VERSION 5.70.0)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)
include(ECMSetupVersion)

find_package(Qt${QT_MAJOR_VERSION} NO_MODULE REQUIRED COMPONENTS Core Gui Widgets Quick)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Config DocTools KIO I18n Purpose Notifications)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GStreamer IMPORTED_TARGET gstreamer-1.0)
pkg_check_modules(GStreamerVideo IMPORTED_TARGET gstreamer-video-1.0)

find_package(KF5Kirigami2)
set_package_properties(KF5Kirigami2 PROPERTIES
    DESCRIPTION "A QtQuick based components set"
    PURPOSE "Required at runtime"
    TYPE RUNTIME
)

include(ECMQMLModules)
ecm_find_qmlmodule(QtGraphicalEffects 1.0)
ecm_find_qmlmodule(QtQml 2.2)
ecm_find_qmlmodule(QtQuick 2.5)
ecm_find_qmlmodule(QtQuick.Controls 2.2)
ecm_find_qmlmodule(QtQuick.Dialogs 1.0)
ecm_find_qmlmodule(QtQuick.Layouts 1.1)
ecm_find_qmlmodule(QtQuick.Window 2.2)

add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(doc)

ki18n_install(po)
kdoctools_install(po)

install(FILES org.kde.kamoso.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
