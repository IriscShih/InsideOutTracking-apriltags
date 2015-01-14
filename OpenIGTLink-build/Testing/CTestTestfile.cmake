# CMake generated Testfile for 
# Source directory: /Users/pearl790131/apriltags/OpenIGTLink/Testing
# Build directory: /Users/pearl790131/apriltags/OpenIGTLink-build/Testing
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(igtlSocketTest "/Users/pearl790131/apriltags/OpenIGTLink-build/bin/igtlSocketTest")
ADD_TEST(igtlMultiThreaderTest1 "/Users/pearl790131/apriltags/OpenIGTLink-build/bin/igtlMultiThreaderTest1")
ADD_TEST(igtlMultiThreaderTest2 "/Users/pearl790131/apriltags/OpenIGTLink-build/bin/igtlMultiThreaderTest2")
ADD_TEST(igtlMultiThreaderTest3 "/Users/pearl790131/apriltags/OpenIGTLink-build/bin/igtlMultiThreaderTest3")
ADD_TEST(igtlMessageFactoryTest "/Users/pearl790131/apriltags/OpenIGTLink-build/bin/igtlMessageFactoryTest")
ADD_TEST(igtlTimeStampTest1 "/Users/pearl790131/apriltags/OpenIGTLink-build/bin/igtlTimeStampTest1")
SUBDIRS(igtlutil)
