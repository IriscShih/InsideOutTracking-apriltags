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
include Examples/Thread/CMakeFiles/SpawnThread.dir/depend.make

# Include the progress variables for this target.
include Examples/Thread/CMakeFiles/SpawnThread.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/Thread/CMakeFiles/SpawnThread.dir/flags.make

Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o: Examples/Thread/CMakeFiles/SpawnThread.dir/flags.make
Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/Thread/SpawnThread.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SpawnThread.dir/SpawnThread.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/Thread/SpawnThread.cxx

Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SpawnThread.dir/SpawnThread.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/Thread/SpawnThread.cxx > CMakeFiles/SpawnThread.dir/SpawnThread.i

Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SpawnThread.dir/SpawnThread.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/Thread/SpawnThread.cxx -o CMakeFiles/SpawnThread.dir/SpawnThread.s

Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.requires:
.PHONY : Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.requires

Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.provides: Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.requires
	$(MAKE) -f Examples/Thread/CMakeFiles/SpawnThread.dir/build.make Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.provides.build
.PHONY : Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.provides

Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.provides.build: Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o

# Object files for target SpawnThread
SpawnThread_OBJECTS = \
"CMakeFiles/SpawnThread.dir/SpawnThread.o"

# External object files for target SpawnThread
SpawnThread_EXTERNAL_OBJECTS =

bin/SpawnThread: Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o
bin/SpawnThread: Examples/Thread/CMakeFiles/SpawnThread.dir/build.make
bin/SpawnThread: bin/libOpenIGTLink.a
bin/SpawnThread: Examples/Thread/CMakeFiles/SpawnThread.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/SpawnThread"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SpawnThread.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/Thread/CMakeFiles/SpawnThread.dir/build: bin/SpawnThread
.PHONY : Examples/Thread/CMakeFiles/SpawnThread.dir/build

Examples/Thread/CMakeFiles/SpawnThread.dir/requires: Examples/Thread/CMakeFiles/SpawnThread.dir/SpawnThread.o.requires
.PHONY : Examples/Thread/CMakeFiles/SpawnThread.dir/requires

Examples/Thread/CMakeFiles/SpawnThread.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread && $(CMAKE_COMMAND) -P CMakeFiles/SpawnThread.dir/cmake_clean.cmake
.PHONY : Examples/Thread/CMakeFiles/SpawnThread.dir/clean

Examples/Thread/CMakeFiles/SpawnThread.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/Thread /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Thread/CMakeFiles/SpawnThread.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/Thread/CMakeFiles/SpawnThread.dir/depend

