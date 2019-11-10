cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(KDE_APPLICATIONS_VERSION_MAJOR "20")
set(KDE_APPLICATIONS_VERSION_MINOR "03")
set(KDE_APPLICATIONS_VERSION_MICRO "70")
set(KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

project(Kamoso VERSION ${KDE_APPLICATIONS_VERSION})

set(KF5_MIN_VERSION 5.63.0)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5 NO_MODULE REQUIRED COMPONENTS Core Gui Widgets Quick Test OpenGL)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Config DocTools KIO I18n Purpose Notifications)

find_package(GStreamer 1.1.90 REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(GObject REQUIRED)

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
ecm_find_qmlmodule(QtQuick.Controls 1.2)
ecm_find_qmlmodule(QtQuick.Dialogs 1.0)
ecm_find_qmlmodule(QtQuick.Layouts 1.1)
ecm_find_qmlmodule(QtQuick.Window 2.2)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)
include(ECMSetupVersion)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(doc)

install(FILES org.kde.kamoso.appdata.xml DESTINATION ${CMAKE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
