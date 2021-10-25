# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/halilibrahim/hydrosheds-cpp-api

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/halilibrahim/hydrosheds-cpp-api/tests

# Include any dependencies generated for this target.
include src/CMakeFiles/hydrosheds.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/hydrosheds.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/hydrosheds.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/hydrosheds.dir/flags.make

src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.o: src/CMakeFiles/hydrosheds.dir/flags.make
src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.o: ../src/hydrosheds.cc
src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.o: src/CMakeFiles/hydrosheds.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/halilibrahim/hydrosheds-cpp-api/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.o"
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.o -MF CMakeFiles/hydrosheds.dir/hydrosheds.cc.o.d -o CMakeFiles/hydrosheds.dir/hydrosheds.cc.o -c /Users/halilibrahim/hydrosheds-cpp-api/src/hydrosheds.cc

src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hydrosheds.dir/hydrosheds.cc.i"
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/halilibrahim/hydrosheds-cpp-api/src/hydrosheds.cc > CMakeFiles/hydrosheds.dir/hydrosheds.cc.i

src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hydrosheds.dir/hydrosheds.cc.s"
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/halilibrahim/hydrosheds-cpp-api/src/hydrosheds.cc -o CMakeFiles/hydrosheds.dir/hydrosheds.cc.s

# Object files for target hydrosheds
hydrosheds_OBJECTS = \
"CMakeFiles/hydrosheds.dir/hydrosheds.cc.o"

# External object files for target hydrosheds
hydrosheds_EXTERNAL_OBJECTS =

src/libhydrosheds.a: src/CMakeFiles/hydrosheds.dir/hydrosheds.cc.o
src/libhydrosheds.a: src/CMakeFiles/hydrosheds.dir/build.make
src/libhydrosheds.a: src/CMakeFiles/hydrosheds.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/halilibrahim/hydrosheds-cpp-api/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libhydrosheds.a"
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/src && $(CMAKE_COMMAND) -P CMakeFiles/hydrosheds.dir/cmake_clean_target.cmake
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hydrosheds.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/hydrosheds.dir/build: src/libhydrosheds.a
.PHONY : src/CMakeFiles/hydrosheds.dir/build

src/CMakeFiles/hydrosheds.dir/clean:
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/src && $(CMAKE_COMMAND) -P CMakeFiles/hydrosheds.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/hydrosheds.dir/clean

src/CMakeFiles/hydrosheds.dir/depend:
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/halilibrahim/hydrosheds-cpp-api /Users/halilibrahim/hydrosheds-cpp-api/src /Users/halilibrahim/hydrosheds-cpp-api/tests /Users/halilibrahim/hydrosheds-cpp-api/tests/src /Users/halilibrahim/hydrosheds-cpp-api/tests/src/CMakeFiles/hydrosheds.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/hydrosheds.dir/depend

