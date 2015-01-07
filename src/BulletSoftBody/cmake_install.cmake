# Install script for directory: /Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/bullet3/src/BulletSoftBody

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Library/Frameworks")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/bullet3/src/src/BulletSoftBody/BulletSoftBody.framework" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBody.framework/Versions/2.83/BulletSoftBody" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBody.framework/Versions/2.83/BulletSoftBody")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "/Library/Frameworks/BulletSoftBody.framework/Versions/2.83/BulletSoftBody"
      -change "/Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/bullet3/src/src/BulletCollision/BulletCollision.framework/Versions/2.83/BulletCollision" "/Library/Frameworks/BulletCollision.framework/Versions/2.83/BulletCollision"
      -change "/Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/bullet3/src/src/BulletDynamics/BulletDynamics.framework/Versions/2.83/BulletDynamics" "/Library/Frameworks/BulletDynamics.framework/Versions/2.83/BulletDynamics"
      -change "/Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/bullet3/src/src/LinearMath/LinearMath.framework/Versions/2.83/LinearMath" "/Library/Frameworks/LinearMath.framework/Versions/2.83/LinearMath"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBody.framework/Versions/2.83/BulletSoftBody")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./BulletSoftBody.framework/Versions/2.83/BulletSoftBody")
    endif()
  endif()
endif()

