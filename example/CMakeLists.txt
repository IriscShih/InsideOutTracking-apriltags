cmake_minimum_required (VERSION 2.6)

link_libraries(apriltags)

#add_library(apriltags_demo libOpenIGTLink.a libigtutil.a)
#pods_install_library(apriltags_demo)

add_executable(apriltags_demo apriltags_demo.cpp Serial.cpp)
pods_install_executables(apriltags_demo)

add_executable(imu imu.cpp Serial.cpp)
pods_install_executables(imu)
