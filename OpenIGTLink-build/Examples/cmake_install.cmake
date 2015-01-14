# Install script for directory: /Users/pearl790131/apriltags/OpenIGTLink/Examples

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
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

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Status/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Point/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/QuaternionTrackingData/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageDatabaseServer/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Bind/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/PolyData/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Capability/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Trajectory/cmake_install.cmake")
  INCLUDE("/Users/pearl790131/apriltags/OpenIGTLink-build/Examples/SessionManager/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

