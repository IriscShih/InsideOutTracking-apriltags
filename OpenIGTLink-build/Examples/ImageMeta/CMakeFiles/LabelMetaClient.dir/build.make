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
include Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/depend.make

# Include the progress variables for this target.
include Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/flags.make

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/flags.make
Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o: /Users/pearl790131/apriltags/OpenIGTLink/Examples/ImageMeta/LabelMetaClient.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/pearl790131/apriltags/OpenIGTLink-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o -c /Users/pearl790131/apriltags/OpenIGTLink/Examples/ImageMeta/LabelMetaClient.cxx

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LabelMetaClient.dir/LabelMetaClient.i"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/pearl790131/apriltags/OpenIGTLink/Examples/ImageMeta/LabelMetaClient.cxx > CMakeFiles/LabelMetaClient.dir/LabelMetaClient.i

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LabelMetaClient.dir/LabelMetaClient.s"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/pearl790131/apriltags/OpenIGTLink/Examples/ImageMeta/LabelMetaClient.cxx -o CMakeFiles/LabelMetaClient.dir/LabelMetaClient.s

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.requires:
.PHONY : Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.requires

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.provides: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.requires
	$(MAKE) -f Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/build.make Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.provides.build
.PHONY : Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.provides

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.provides.build: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o

# Object files for target LabelMetaClient
LabelMetaClient_OBJECTS = \
"CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o"

# External object files for target LabelMetaClient
LabelMetaClient_EXTERNAL_OBJECTS =

bin/LabelMetaClient: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o
bin/LabelMetaClient: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/build.make
bin/LabelMetaClient: bin/libOpenIGTLink.a
bin/LabelMetaClient: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/LabelMetaClient"
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LabelMetaClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/build: bin/LabelMetaClient
.PHONY : Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/build

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/requires: Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/LabelMetaClient.o.requires
.PHONY : Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/requires

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/clean:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta && $(CMAKE_COMMAND) -P CMakeFiles/LabelMetaClient.dir/cmake_clean.cmake
.PHONY : Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/clean

Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/depend:
	cd /Users/pearl790131/apriltags/OpenIGTLink-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pearl790131/apriltags/OpenIGTLink /Users/pearl790131/apriltags/OpenIGTLink/Examples/ImageMeta /Users/pearl790131/apriltags/OpenIGTLink-build /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta /Users/pearl790131/apriltags/OpenIGTLink-build/Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/ImageMeta/CMakeFiles/LabelMetaClient.dir/depend

