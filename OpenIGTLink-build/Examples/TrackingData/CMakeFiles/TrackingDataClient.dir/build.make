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
include Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/depend.make

# Include the progress variables for this target.
include Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/flags.make

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/flags.make
Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/TrackingData/TrackingDataClient.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/TrackingData/TrackingDataClient.cxx

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TrackingDataClient.dir/TrackingDataClient.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/TrackingData/TrackingDataClient.cxx > CMakeFiles/TrackingDataClient.dir/TrackingDataClient.i

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TrackingDataClient.dir/TrackingDataClient.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/TrackingData/TrackingDataClient.cxx -o CMakeFiles/TrackingDataClient.dir/TrackingDataClient.s

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.requires:
.PHONY : Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.requires

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.provides: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.requires
	$(MAKE) -f Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/build.make Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.provides.build
.PHONY : Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.provides

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.provides.build: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o

# Object files for target TrackingDataClient
TrackingDataClient_OBJECTS = \
"CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o"

# External object files for target TrackingDataClient
TrackingDataClient_EXTERNAL_OBJECTS =

bin/TrackingDataClient: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o
bin/TrackingDataClient: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/build.make
bin/TrackingDataClient: bin/libOpenIGTLink.a
bin/TrackingDataClient: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/TrackingDataClient"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TrackingDataClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/build: bin/TrackingDataClient
.PHONY : Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/build

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/requires: Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/TrackingDataClient.o.requires
.PHONY : Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/requires

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData && $(CMAKE_COMMAND) -P CMakeFiles/TrackingDataClient.dir/cmake_clean.cmake
.PHONY : Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/clean

Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/TrackingData /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/TrackingData/CMakeFiles/TrackingDataClient.dir/depend
