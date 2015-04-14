project(Kamoso)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5 NO_MODULE REQUIRED COMPONENTS Core Gui Widgets Quick Network OpenGL DBus Test)
find_package(KF5 REQUIRED COMPONENTS WidgetsAddons Config ConfigWidgets DocTools Solid KIO Declarative I18n)

find_package(Qt5GStreamer 1.1.90 REQUIRED)
find_package(KF5Purpose REQUIRED)
find_package(UDev REQUIRED)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddTests)
include(FeatureSummary)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(doc)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
