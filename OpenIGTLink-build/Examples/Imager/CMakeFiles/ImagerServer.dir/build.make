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
include Examples/Imager/CMakeFiles/ImagerServer.dir/depend.make

# Include the progress variables for this target.
include Examples/Imager/CMakeFiles/ImagerServer.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/Imager/CMakeFiles/ImagerServer.dir/flags.make

Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o: Examples/Imager/CMakeFiles/ImagerServer.dir/flags.make
Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerServer.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ImagerServer.dir/ImagerServer.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerServer.cxx

Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImagerServer.dir/ImagerServer.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerServer.cxx > CMakeFiles/ImagerServer.dir/ImagerServer.i

Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImagerServer.dir/ImagerServer.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerServer.cxx -o CMakeFiles/ImagerServer.dir/ImagerServer.s

Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.requires:
.PHONY : Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.requires

Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.provides: Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.requires
	$(MAKE) -f Examples/Imager/CMakeFiles/ImagerServer.dir/build.make Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.provides.build
.PHONY : Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.provides

Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.provides.build: Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o

# Object files for target ImagerServer
ImagerServer_OBJECTS = \
"CMakeFiles/ImagerServer.dir/ImagerServer.o"

# External object files for target ImagerServer
ImagerServer_EXTERNAL_OBJECTS =

bin/ImagerServer: Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o
bin/ImagerServer: Examples/Imager/CMakeFiles/ImagerServer.dir/build.make
bin/ImagerServer: bin/libOpenIGTLink.a
bin/ImagerServer: Examples/Imager/CMakeFiles/ImagerServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/ImagerServer"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ImagerServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/Imager/CMakeFiles/ImagerServer.dir/build: bin/ImagerServer
.PHONY : Examples/Imager/CMakeFiles/ImagerServer.dir/build

Examples/Imager/CMakeFiles/ImagerServer.dir/requires: Examples/Imager/CMakeFiles/ImagerServer.dir/ImagerServer.o.requires
.PHONY : Examples/Imager/CMakeFiles/ImagerServer.dir/requires

Examples/Imager/CMakeFiles/ImagerServer.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && $(CMAKE_COMMAND) -P CMakeFiles/ImagerServer.dir/cmake_clean.cmake
.PHONY : Examples/Imager/CMakeFiles/ImagerServer.dir/clean

Examples/Imager/CMakeFiles/ImagerServer.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager/CMakeFiles/ImagerServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/Imager/CMakeFiles/ImagerServer.dir/depend

