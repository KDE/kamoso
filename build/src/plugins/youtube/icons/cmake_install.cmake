# Install script for directory: /home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/nasete/cod3s/cpp/kde/bin")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/apps/kamoso_youtube/icons/hicolor/22x22/actions/." TYPE FILE RENAME "youtube.png" FILES "/home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons/hi22-action-youtube.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/apps/kamoso_youtube/icons/hicolor/16x16/actions/." TYPE FILE RENAME "youtube.png" FILES "/home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons/hi16-action-youtube.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/apps/kamoso_youtube/icons/hicolor/48x48/actions/." TYPE FILE RENAME "youtube.png" FILES "/home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons/hi48-action-youtube.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/apps/kamoso_youtube/icons/hicolor/64x64/actions/." TYPE FILE RENAME "youtube.png" FILES "/home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons/hi64-action-youtube.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/apps/kamoso_youtube/icons/hicolor/32x32/actions/." TYPE FILE RENAME "youtube.png" FILES "/home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons/hi32-action-youtube.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/apps/kamoso_youtube/icons/hicolor/scalable/actions/." TYPE FILE RENAME "youtube.svgz" FILES "/home/nasete/cod3s/cpp/kamoso/src/plugins/youtube/icons/hisc-action-youtube.svgz")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
    set(DESTDIR_VALUE "$ENV{DESTDIR}")
    if (NOT DESTDIR_VALUE)
        file(WRITE "/home/nasete/cod3s/cpp/kde/bin/share/icons/hicolor/temp.txt" "update")
        file(REMOVE "/home/nasete/cod3s/cpp/kde/bin/share/icons/hicolor/temp.txt")
    endif (NOT DESTDIR_VALUE)
    
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

