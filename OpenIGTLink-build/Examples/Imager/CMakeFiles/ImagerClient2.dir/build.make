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
include Examples/Imager/CMakeFiles/ImagerClient2.dir/depend.make

# Include the progress variables for this target.
include Examples/Imager/CMakeFiles/ImagerClient2.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/Imager/CMakeFiles/ImagerClient2.dir/flags.make

Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o: Examples/Imager/CMakeFiles/ImagerClient2.dir/flags.make
Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerClient2.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ImagerClient2.dir/ImagerClient2.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerClient2.cxx

Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImagerClient2.dir/ImagerClient2.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerClient2.cxx > CMakeFiles/ImagerClient2.dir/ImagerClient2.i

Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImagerClient2.dir/ImagerClient2.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager/ImagerClient2.cxx -o CMakeFiles/ImagerClient2.dir/ImagerClient2.s

Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.requires:
.PHONY : Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.requires

Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.provides: Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.requires
	$(MAKE) -f Examples/Imager/CMakeFiles/ImagerClient2.dir/build.make Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.provides.build
.PHONY : Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.provides

Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.provides.build: Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o

# Object files for target ImagerClient2
ImagerClient2_OBJECTS = \
"CMakeFiles/ImagerClient2.dir/ImagerClient2.o"

# External object files for target ImagerClient2
ImagerClient2_EXTERNAL_OBJECTS =

bin/ImagerClient2: Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o
bin/ImagerClient2: Examples/Imager/CMakeFiles/ImagerClient2.dir/build.make
bin/ImagerClient2: bin/libOpenIGTLink.a
bin/ImagerClient2: Examples/Imager/CMakeFiles/ImagerClient2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/ImagerClient2"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ImagerClient2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/Imager/CMakeFiles/ImagerClient2.dir/build: bin/ImagerClient2
.PHONY : Examples/Imager/CMakeFiles/ImagerClient2.dir/build

Examples/Imager/CMakeFiles/ImagerClient2.dir/requires: Examples/Imager/CMakeFiles/ImagerClient2.dir/ImagerClient2.o.requires
.PHONY : Examples/Imager/CMakeFiles/ImagerClient2.dir/requires

Examples/Imager/CMakeFiles/ImagerClient2.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager && $(CMAKE_COMMAND) -P CMakeFiles/ImagerClient2.dir/cmake_clean.cmake
.PHONY : Examples/Imager/CMakeFiles/ImagerClient2.dir/clean

Examples/Imager/CMakeFiles/ImagerClient2.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/Imager /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Imager/CMakeFiles/ImagerClient2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/Imager/CMakeFiles/ImagerClient2.dir/depend
