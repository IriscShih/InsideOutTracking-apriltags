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
include Testing/CMakeFiles/igtlMultiThreaderTest1.dir/depend.make

# Include the progress variables for this target.
include Testing/CMakeFiles/igtlMultiThreaderTest1.dir/progress.make

# Include the compile flags for this target's objects.
include Testing/CMakeFiles/igtlMultiThreaderTest1.dir/flags.make

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/flags.make
Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o: /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlMultiThreaderTest1.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o -c /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlMultiThreaderTest1.cxx

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlMultiThreaderTest1.cxx > CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.i

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Testing/igtlMultiThreaderTest1.cxx -o CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.s

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.requires:
.PHONY : Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.requires

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.provides: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.requires
	$(MAKE) -f Testing/CMakeFiles/igtlMultiThreaderTest1.dir/build.make Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.provides.build
.PHONY : Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.provides

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.provides.build: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o

# Object files for target igtlMultiThreaderTest1
igtlMultiThreaderTest1_OBJECTS = \
"CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o"

# External object files for target igtlMultiThreaderTest1
igtlMultiThreaderTest1_EXTERNAL_OBJECTS =

bin/igtlMultiThreaderTest1: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o
bin/igtlMultiThreaderTest1: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/build.make
bin/igtlMultiThreaderTest1: bin/libOpenIGTLink.a
bin/igtlMultiThreaderTest1: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/igtlMultiThreaderTest1"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/igtlMultiThreaderTest1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Testing/CMakeFiles/igtlMultiThreaderTest1.dir/build: bin/igtlMultiThreaderTest1
.PHONY : Testing/CMakeFiles/igtlMultiThreaderTest1.dir/build

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/requires: Testing/CMakeFiles/igtlMultiThreaderTest1.dir/igtlMultiThreaderTest1.o.requires
.PHONY : Testing/CMakeFiles/igtlMultiThreaderTest1.dir/requires

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Testing && $(CMAKE_COMMAND) -P CMakeFiles/igtlMultiThreaderTest1.dir/cmake_clean.cmake
.PHONY : Testing/CMakeFiles/igtlMultiThreaderTest1.dir/clean

Testing/CMakeFiles/igtlMultiThreaderTest1.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Testing /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Testing /Users/pearl790131/apriltags/OpenIGTLink-build/Testing/CMakeFiles/igtlMultiThreaderTest1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Testing/CMakeFiles/igtlMultiThreaderTest1.dir/depend
