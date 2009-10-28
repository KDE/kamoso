# Install script for directory: /home/nasete/cod3s/cpp/kamoso/src

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
  IF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso"
         RPATH "/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/qt/lib")
  ENDIF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/bin" TYPE EXECUTABLE FILES "/home/nasete/cod3s/cpp/kamoso/build/src/kamoso")
  IF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso"
         OLD_RPATH "/home/nasete/cod3s/cpp/kamoso/build/lib:/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/qt/lib"
         NEW_RPATH "/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/qt/lib")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/bin/kamoso")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so"
         RPATH "/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/qt/lib")
  ENDIF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/lib" TYPE SHARED_LIBRARY FILES "/home/nasete/cod3s/cpp/kamoso/build/lib/libkamosoplugin.so")
  IF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so"
         OLD_RPATH "/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/qt/lib:::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/lib:/home/nasete/cod3s/cpp/kde/bin/qt/lib")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/home/nasete/cod3s/cpp/kde/bin/lib/libkamosoplugin.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/applications/kde4" TYPE FILE FILES "/home/nasete/cod3s/cpp/kamoso/src/kamoso.desktop")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/home/nasete/cod3s/cpp/kde/bin/share/kde4/servicetypes" TYPE FILE FILES "/home/nasete/cod3s/cpp/kamoso/src/kamosoplugin.desktop")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/nasete/cod3s/cpp/kamoso/build/src/plugins/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

