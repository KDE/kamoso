project(Kamoso)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5 NO_MODULE REQUIRED COMPONENTS Core Gui Widgets Quick Network OpenGL DBus)
find_package(KF5 REQUIRED COMPONENTS WidgetsAddons Config ConfigWidgets DocTools Solid KIO Declarative I18n)

# find_package(Kipi REQUIRED)
# find_package(Phonon REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(GStreamer REQUIRED COMPONENTS base)
find_package(Qt5GStreamer REQUIRED)
find_package(UDev)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(FeatureSummary)

# macro_optional_find_package(Nepomuk)
# macro_log_feature(NEPOMUK_FOUND "Nepomuk" "Handles all kinds of metadata on the KDE desktop to provide a semantic desktop" "http://nepomuk.kde.org/" FALSE "" "STRONGLY_RECOMMENDED: Nepomuk is used to provide metadata on objects like files, emails etc. to provide a semantic desktop.")
set(HAVE_NEPOMUK ${NEPOMUK_FOUND})

add_subdirectory(src)
# add_subdirectory(widgetsrc)
add_subdirectory(icons)
add_subdirectory(doc)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
