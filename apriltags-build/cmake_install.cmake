# Install script for directory: /Users/pearl790131/apriltags

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
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
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

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/Debug/libapriltags.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
    ENDIF()
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/Release/libapriltags.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
    ENDIF()
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/MinSizeRel/libapriltags.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
    ENDIF()
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/RelWithDebInfo/libapriltags.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltags.a")
    ENDIF()
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/Debug/libOpenIGTLink.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
    ENDIF()
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/Release/libOpenIGTLink.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
    ENDIF()
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/MinSizeRel/libOpenIGTLink.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
    ENDIF()
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pearl790131/apriltags-build/lib/RelWithDebInfo/libOpenIGTLink.a")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
      EXECUTE_PROCESS(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOpenIGTLink.a")
    ENDIF()
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/AprilTags" TYPE FILE FILES
    "/Users/pearl790131/apriltags/AprilTags/Edge.h"
    "/Users/pearl790131/apriltags/AprilTags/FloatImage.h"
    "/Users/pearl790131/apriltags/AprilTags/Gaussian.h"
    "/Users/pearl790131/apriltags/AprilTags/GLine2D.h"
    "/Users/pearl790131/apriltags/AprilTags/GLineSegment2D.h"
    "/Users/pearl790131/apriltags/AprilTags/GrayModel.h"
    "/Users/pearl790131/apriltags/AprilTags/Gridder.h"
    "/Users/pearl790131/apriltags/AprilTags/Homography33.h"
    "/Users/pearl790131/apriltags/AprilTags/MathUtil.h"
    "/Users/pearl790131/apriltags/AprilTags/pch.h"
    "/Users/pearl790131/apriltags/AprilTags/Quad.h"
    "/Users/pearl790131/apriltags/AprilTags/Segment.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag16h5.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag16h5_other.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag25h7.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag25h9.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag36h11.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag36h11_other.h"
    "/Users/pearl790131/apriltags/AprilTags/Tag36h9.h"
    "/Users/pearl790131/apriltags/AprilTags/TagDetection.h"
    "/Users/pearl790131/apriltags/AprilTags/TagDetector.h"
    "/Users/pearl790131/apriltags/AprilTags/TagFamily.h"
    "/Users/pearl790131/apriltags/AprilTags/UnionFindSimple.h"
    "/Users/pearl790131/apriltags/AprilTags/XYWeight.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/igtl" TYPE FILE FILES
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlBindMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlCapabilityMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlClientSocket.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlColorTableMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlConditionVariable.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlCreateObjectFunction.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlFastMutexLock.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlImageMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlImageMessage2.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlImageMetaMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlLabelMetaMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlLightObject.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMacro.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMath.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMessageBase.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMessageFactory.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMessageHandler.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMessageHandlerMacro.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMessageHeader.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMultiThreader.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlMutexLock.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlNDArrayMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlObject.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlObjectFactory.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlObjectFactoryBase.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlOSUtil.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlPointMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlPolyDataMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlPositionMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlQuaternionTrackingDataMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlSensorMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlServerSocket.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlSessionManager.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlSimpleFastMutexLock.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlSmartPointer.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlSocket.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlStatusMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlStringMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlTimeStamp.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlTrackingDataMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlTrajectoryMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlTransformMessage.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlTypes.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlUnit.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlWin32Header.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlWindows.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenIGTLink/Source/igtlutil" TYPE FILE FILES
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_bind.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_capability.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_colortable.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_header.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_image.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_imgmeta.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_lbmeta.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_ndarray.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_point.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_polydata.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_position.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_qtdata.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_qtrans.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_sensor.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_status.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_string.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_tdata.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_trajectory.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_transform.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_types.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_unit.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_util.h"
    "/Users/pearl790131/apriltags/OpenIGTLink/Source/igtlutil/igtl_win32header.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/pearl790131/apriltags-build/lib/pkgconfig/apriltags.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/Users/pearl790131/apriltags-build/example/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/Users/pearl790131/apriltags-build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/Users/pearl790131/apriltags-build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
