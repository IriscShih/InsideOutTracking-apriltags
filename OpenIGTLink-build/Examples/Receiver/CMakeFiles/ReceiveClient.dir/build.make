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
include Examples/Receiver/CMakeFiles/ReceiveClient.dir/depend.make

# Include the progress variables for this target.
include Examples/Receiver/CMakeFiles/ReceiveClient.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/Receiver/CMakeFiles/ReceiveClient.dir/flags.make

Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o: Examples/Receiver/CMakeFiles/ReceiveClient.dir/flags.make
Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/Receiver/ReceiveClient.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ReceiveClient.dir/ReceiveClient.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/Receiver/ReceiveClient.cxx

Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ReceiveClient.dir/ReceiveClient.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/Receiver/ReceiveClient.cxx > CMakeFiles/ReceiveClient.dir/ReceiveClient.i

Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ReceiveClient.dir/ReceiveClient.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/Receiver/ReceiveClient.cxx -o CMakeFiles/ReceiveClient.dir/ReceiveClient.s

Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.requires:
.PHONY : Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.requires

Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.provides: Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.requires
	$(MAKE) -f Examples/Receiver/CMakeFiles/ReceiveClient.dir/build.make Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.provides.build
.PHONY : Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.provides

Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.provides.build: Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o

# Object files for target ReceiveClient
ReceiveClient_OBJECTS = \
"CMakeFiles/ReceiveClient.dir/ReceiveClient.o"

# External object files for target ReceiveClient
ReceiveClient_EXTERNAL_OBJECTS =

bin/ReceiveClient: Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o
bin/ReceiveClient: Examples/Receiver/CMakeFiles/ReceiveClient.dir/build.make
bin/ReceiveClient: bin/libOpenIGTLink.a
bin/ReceiveClient: Examples/Receiver/CMakeFiles/ReceiveClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/ReceiveClient"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ReceiveClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/Receiver/CMakeFiles/ReceiveClient.dir/build: bin/ReceiveClient
.PHONY : Examples/Receiver/CMakeFiles/ReceiveClient.dir/build

Examples/Receiver/CMakeFiles/ReceiveClient.dir/requires: Examples/Receiver/CMakeFiles/ReceiveClient.dir/ReceiveClient.o.requires
.PHONY : Examples/Receiver/CMakeFiles/ReceiveClient.dir/requires

Examples/Receiver/CMakeFiles/ReceiveClient.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver && $(CMAKE_COMMAND) -P CMakeFiles/ReceiveClient.dir/cmake_clean.cmake
.PHONY : Examples/Receiver/CMakeFiles/ReceiveClient.dir/clean

Examples/Receiver/CMakeFiles/ReceiveClient.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/Receiver /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/Receiver/CMakeFiles/ReceiveClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/Receiver/CMakeFiles/ReceiveClient.dir/depend

