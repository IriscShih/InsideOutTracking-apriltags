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
include Examples/String/CMakeFiles/StringServer.dir/depend.make

# Include the progress variables for this target.
include Examples/String/CMakeFiles/StringServer.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/String/CMakeFiles/StringServer.dir/flags.make

Examples/String/CMakeFiles/StringServer.dir/StringServer.o: Examples/String/CMakeFiles/StringServer.dir/flags.make
Examples/String/CMakeFiles/StringServer.dir/StringServer.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/String/StringServer.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/String/CMakeFiles/StringServer.dir/StringServer.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/StringServer.dir/StringServer.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/String/StringServer.cxx

Examples/String/CMakeFiles/StringServer.dir/StringServer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StringServer.dir/StringServer.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/String/StringServer.cxx > CMakeFiles/StringServer.dir/StringServer.i

Examples/String/CMakeFiles/StringServer.dir/StringServer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StringServer.dir/StringServer.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/String/StringServer.cxx -o CMakeFiles/StringServer.dir/StringServer.s

Examples/String/CMakeFiles/StringServer.dir/StringServer.o.requires:
.PHONY : Examples/String/CMakeFiles/StringServer.dir/StringServer.o.requires

Examples/String/CMakeFiles/StringServer.dir/StringServer.o.provides: Examples/String/CMakeFiles/StringServer.dir/StringServer.o.requires
	$(MAKE) -f Examples/String/CMakeFiles/StringServer.dir/build.make Examples/String/CMakeFiles/StringServer.dir/StringServer.o.provides.build
.PHONY : Examples/String/CMakeFiles/StringServer.dir/StringServer.o.provides

Examples/String/CMakeFiles/StringServer.dir/StringServer.o.provides.build: Examples/String/CMakeFiles/StringServer.dir/StringServer.o

# Object files for target StringServer
StringServer_OBJECTS = \
"CMakeFiles/StringServer.dir/StringServer.o"

# External object files for target StringServer
StringServer_EXTERNAL_OBJECTS =

bin/StringServer: Examples/String/CMakeFiles/StringServer.dir/StringServer.o
bin/StringServer: Examples/String/CMakeFiles/StringServer.dir/build.make
bin/StringServer: bin/libOpenIGTLink.a
bin/StringServer: Examples/String/CMakeFiles/StringServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/StringServer"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/StringServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/String/CMakeFiles/StringServer.dir/build: bin/StringServer
.PHONY : Examples/String/CMakeFiles/StringServer.dir/build

Examples/String/CMakeFiles/StringServer.dir/requires: Examples/String/CMakeFiles/StringServer.dir/StringServer.o.requires
.PHONY : Examples/String/CMakeFiles/StringServer.dir/requires

Examples/String/CMakeFiles/StringServer.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String && $(CMAKE_COMMAND) -P CMakeFiles/StringServer.dir/cmake_clean.cmake
.PHONY : Examples/String/CMakeFiles/StringServer.dir/clean

Examples/String/CMakeFiles/StringServer.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/String /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/String/CMakeFiles/StringServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/String/CMakeFiles/StringServer.dir/depend

