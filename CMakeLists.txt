cmake_minimum_required(VERSION 2.6)
project(apriltags)

#add_definitions(-pg) #"-fopenmp)

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME apriltags)
include(cmake/pods.cmake)

file(GLOB SOURCE "src/*.cc")
include_directories(AprilTags .)
add_library(apriltags ${SOURCE})

file(GLOB STUPID "OpenIGTLink/Source/*.cxx")
include_directories(OpenIGTLink/Source .)
include_directories(igtl . /include/igtl)
include_directories(igtl . /lib/igtl)
add_library(OpenIGTLink ${STUPID})

file(GLOB STUPID2 "OpenIGTLink/Source/igtlutil/*.c")
include_directories(OpenIGTLink/Source/igtlutil .)
add_library(igtlutil ${STUPID2})

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(apriltags ${OpenCV_LIBS}) #-pg) #-fopenmp)
pods_use_pkg_config_packages(apriltags eigen3)


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(apriltags -L/Users/pearl790131/apriltags/pod-build/lib)
else (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  pods_use_pkg_config_packages(apriltags libv4l2)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

pods_install_libraries(apriltags)
pods_install_libraries(OpenIGTLink)


file(GLOB header_files "AprilTags/*.h")
pods_install_headers(${header_files} DESTINATION AprilTags/)

file(GLOB op_header "OpenIGTLink/Source/*.h")
pods_install_headers(${op_header} DESTINATION igtl/)

file(GLOB op_header2 "OpenIGTLink/Source/igtlutil/*.h")
pods_install_headers(${op_header2} DESTINATION OpenIGTLink/Source/igtlutil/)


pods_install_pkg_config_file(apriltags
    LIBS -lapriltags
    REQUIRES eigen3 opencv2 
    VERSION 1.0.0)


add_subdirectory(example)
