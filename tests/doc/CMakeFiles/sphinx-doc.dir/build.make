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

# Utility rule file for sphinx-doc.

# Include any custom commands dependencies for this target.
include doc/CMakeFiles/sphinx-doc.dir/compiler_depend.make

# Include the progress variables for this target.
include doc/CMakeFiles/sphinx-doc.dir/progress.make

doc/CMakeFiles/sphinx-doc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/halilibrahim/hydrosheds-cpp-api/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating documentation with Sphinx..."
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/doc && sphinx-build -b html -Dbreathe_projects.hydrosheds="/Users/halilibrahim/hydrosheds-cpp-api/tests/doc/xml" -c /Users/halilibrahim/hydrosheds-cpp-api/doc /Users/halilibrahim/hydrosheds-cpp-api/doc /Users/halilibrahim/hydrosheds-cpp-api/tests/doc/sphinx

sphinx-doc: doc/CMakeFiles/sphinx-doc
sphinx-doc: doc/CMakeFiles/sphinx-doc.dir/build.make
.PHONY : sphinx-doc

# Rule to build all files generated by this target.
doc/CMakeFiles/sphinx-doc.dir/build: sphinx-doc
.PHONY : doc/CMakeFiles/sphinx-doc.dir/build

doc/CMakeFiles/sphinx-doc.dir/clean:
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests/doc && $(CMAKE_COMMAND) -P CMakeFiles/sphinx-doc.dir/cmake_clean.cmake
.PHONY : doc/CMakeFiles/sphinx-doc.dir/clean

doc/CMakeFiles/sphinx-doc.dir/depend:
	cd /Users/halilibrahim/hydrosheds-cpp-api/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/halilibrahim/hydrosheds-cpp-api /Users/halilibrahim/hydrosheds-cpp-api/doc /Users/halilibrahim/hydrosheds-cpp-api/tests /Users/halilibrahim/hydrosheds-cpp-api/tests/doc /Users/halilibrahim/hydrosheds-cpp-api/tests/doc/CMakeFiles/sphinx-doc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/CMakeFiles/sphinx-doc.dir/depend

