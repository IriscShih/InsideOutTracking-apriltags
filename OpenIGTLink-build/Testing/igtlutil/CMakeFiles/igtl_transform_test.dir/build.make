# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Applications/CMake 2.8-12.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-12.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-12.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/pearl790131/apriltags/OpenIGTLink

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/pearl790131/apriltags/OpenIGTLink-build

# Include any dependencies generated for this target.
include Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/depend.make

# Include the progress variables for this target.
include Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/progress.make

# Include the compile flags for this target's objects.
include Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/flags.make

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/flags.make
Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o: /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlutil/igtl_transform_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o   -c /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlutil/igtl_transform_test.c

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/igtl_transform_test.dir/igtl_transform_test.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlutil/igtl_transform_test.c > CMakeFiles/igtl_transform_test.dir/igtl_transform_test.i

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/igtl_transform_test.dir/igtl_transform_test.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlutil/igtl_transform_test.c -o CMakeFiles/igtl_transform_test.dir/igtl_transform_test.s

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.requires:
.PHONY : Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.requires

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.provides: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.requires
	$(MAKE) -f Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/build.make Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.provides.build
.PHONY : Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.provides

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.provides.build: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o

# Object files for target igtl_transform_test
igtl_transform_test_OBJECTS = \
"CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o"

# External object files for target igtl_transform_test
igtl_transform_test_EXTERNAL_OBJECTS =

bin/igtl_transform_test: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o
bin/igtl_transform_test: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/build.make
bin/igtl_transform_test: bin/libOpenIGTLink.a
bin/igtl_transform_test: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/igtl_transform_test"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/igtl_transform_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/build: bin/igtl_transform_test
.PHONY : Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/build

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/requires: Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/igtl_transform_test.o.requires
.PHONY : Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/requires

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil && $(CMAKE_COMMAND) -P CMakeFiles/igtl_transform_test.dir/cmake_clean.cmake
.PHONY : Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/clean

Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlutil /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Testing/igtlutil/CMakeFiles/igtl_transform_test.dir/depend

