project(Kamoso)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5 NO_MODULE REQUIRED COMPONENTS Core Gui Widgets Quick Test OpenGL)
find_package(KF5 REQUIRED COMPONENTS Config DocTools KIO I18n Purpose)

find_package(GStreamer 1.1.90 REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(GObject REQUIRED)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(doc)

install(FILES org.kde.kamoso.appdata.xml DESTINATION ${CMAKE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
