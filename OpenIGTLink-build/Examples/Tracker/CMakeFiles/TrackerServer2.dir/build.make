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
include Examples/Tracker/CMakeFiles/TrackerServer2.dir/depend.make

# Include the progress variables for this target.
include Examples/Tracker/CMakeFiles/TrackerServer2.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/Tracker/CMakeFiles/TrackerServer2.dir/flags.make

Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o: Examples/Tracker/CMakeFiles/TrackerServer2.dir/flags.make
Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/Tracker/TrackerServer2.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TrackerServer2.dir/TrackerServer2.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/Tracker/TrackerServer2.cxx

Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TrackerServer2.dir/TrackerServer2.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/Tracker/TrackerServer2.cxx > CMakeFiles/TrackerServer2.dir/TrackerServer2.i

Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TrackerServer2.dir/TrackerServer2.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/Tracker/TrackerServer2.cxx -o CMakeFiles/TrackerServer2.dir/TrackerServer2.s

Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.requires:
.PHONY : Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.requires

Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.provides: Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.requires
	$(MAKE) -f Examples/Tracker/CMakeFiles/TrackerServer2.dir/build.make Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.provides.build
.PHONY : Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.provides

Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.provides.build: Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o

# Object files for target TrackerServer2
TrackerServer2_OBJECTS = \
"CMakeFiles/TrackerServer2.dir/TrackerServer2.o"

# External object files for target TrackerServer2
TrackerServer2_EXTERNAL_OBJECTS =

bin/TrackerServer2: Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o
bin/TrackerServer2: Examples/Tracker/CMakeFiles/TrackerServer2.dir/build.make
bin/TrackerServer2: bin/libOpenIGTLink.a
bin/TrackerServer2: Examples/Tracker/CMakeFiles/TrackerServer2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/TrackerServer2"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TrackerServer2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/Tracker/CMakeFiles/TrackerServer2.dir/build: bin/TrackerServer2
.PHONY : Examples/Tracker/CMakeFiles/TrackerServer2.dir/build

Examples/Tracker/CMakeFiles/TrackerServer2.dir/requires: Examples/Tracker/CMakeFiles/TrackerServer2.dir/TrackerServer2.o.requires
.PHONY : Examples/Tracker/CMakeFiles/TrackerServer2.dir/requires

Examples/Tracker/CMakeFiles/TrackerServer2.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker && $(CMAKE_COMMAND) -P CMakeFiles/TrackerServer2.dir/cmake_clean.cmake
.PHONY : Examples/Tracker/CMakeFiles/TrackerServer2.dir/clean

Examples/Tracker/CMakeFiles/TrackerServer2.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/Tracker /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Tracker/CMakeFiles/TrackerServer2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/Tracker/CMakeFiles/TrackerServer2.dir/depend

