# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.0.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.0.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/admin/Project/speech/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/admin/Project/speech/src

# Include any dependencies generated for this target.
include EditDistance/CMakeFiles/EditDistance.dir/depend.make

# Include the progress variables for this target.
include EditDistance/CMakeFiles/EditDistance.dir/progress.make

# Include the compile flags for this target's objects.
include EditDistance/CMakeFiles/EditDistance.dir/flags.make

EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o: EditDistance/CMakeFiles/EditDistance.dir/flags.make
EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o: EditDistance/calcDist.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/admin/Project/speech/src/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/EditDistance.dir/calcDist.cpp.o -c /Users/admin/Project/speech/src/EditDistance/calcDist.cpp

EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EditDistance.dir/calcDist.cpp.i"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/admin/Project/speech/src/EditDistance/calcDist.cpp > CMakeFiles/EditDistance.dir/calcDist.cpp.i

EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EditDistance.dir/calcDist.cpp.s"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/admin/Project/speech/src/EditDistance/calcDist.cpp -o CMakeFiles/EditDistance.dir/calcDist.cpp.s

EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.requires:
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.requires

EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.provides: EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.requires
	$(MAKE) -f EditDistance/CMakeFiles/EditDistance.dir/build.make EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.provides.build
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.provides

EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.provides.build: EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o

EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o: EditDistance/CMakeFiles/EditDistance.dir/flags.make
EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o: EditDistance/stringtool.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/admin/Project/speech/src/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/EditDistance.dir/stringtool.cpp.o -c /Users/admin/Project/speech/src/EditDistance/stringtool.cpp

EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EditDistance.dir/stringtool.cpp.i"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/admin/Project/speech/src/EditDistance/stringtool.cpp > CMakeFiles/EditDistance.dir/stringtool.cpp.i

EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EditDistance.dir/stringtool.cpp.s"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/admin/Project/speech/src/EditDistance/stringtool.cpp -o CMakeFiles/EditDistance.dir/stringtool.cpp.s

EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.requires:
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.requires

EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.provides: EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.requires
	$(MAKE) -f EditDistance/CMakeFiles/EditDistance.dir/build.make EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.provides.build
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.provides

EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.provides.build: EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o

EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o: EditDistance/CMakeFiles/EditDistance.dir/flags.make
EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o: EditDistance/app.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/admin/Project/speech/src/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/EditDistance.dir/app.cpp.o -c /Users/admin/Project/speech/src/EditDistance/app.cpp

EditDistance/CMakeFiles/EditDistance.dir/app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EditDistance.dir/app.cpp.i"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/admin/Project/speech/src/EditDistance/app.cpp > CMakeFiles/EditDistance.dir/app.cpp.i

EditDistance/CMakeFiles/EditDistance.dir/app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EditDistance.dir/app.cpp.s"
	cd /Users/admin/Project/speech/src/EditDistance && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/admin/Project/speech/src/EditDistance/app.cpp -o CMakeFiles/EditDistance.dir/app.cpp.s

EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.requires:
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.requires

EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.provides: EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.requires
	$(MAKE) -f EditDistance/CMakeFiles/EditDistance.dir/build.make EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.provides.build
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.provides

EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.provides.build: EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o

# Object files for target EditDistance
EditDistance_OBJECTS = \
"CMakeFiles/EditDistance.dir/calcDist.cpp.o" \
"CMakeFiles/EditDistance.dir/stringtool.cpp.o" \
"CMakeFiles/EditDistance.dir/app.cpp.o"

# External object files for target EditDistance
EditDistance_EXTERNAL_OBJECTS =

EditDistance/libEditDistance.a: EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o
EditDistance/libEditDistance.a: EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o
EditDistance/libEditDistance.a: EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o
EditDistance/libEditDistance.a: EditDistance/CMakeFiles/EditDistance.dir/build.make
EditDistance/libEditDistance.a: EditDistance/CMakeFiles/EditDistance.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libEditDistance.a"
	cd /Users/admin/Project/speech/src/EditDistance && $(CMAKE_COMMAND) -P CMakeFiles/EditDistance.dir/cmake_clean_target.cmake
	cd /Users/admin/Project/speech/src/EditDistance && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EditDistance.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
EditDistance/CMakeFiles/EditDistance.dir/build: EditDistance/libEditDistance.a
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/build

EditDistance/CMakeFiles/EditDistance.dir/requires: EditDistance/CMakeFiles/EditDistance.dir/calcDist.cpp.o.requires
EditDistance/CMakeFiles/EditDistance.dir/requires: EditDistance/CMakeFiles/EditDistance.dir/stringtool.cpp.o.requires
EditDistance/CMakeFiles/EditDistance.dir/requires: EditDistance/CMakeFiles/EditDistance.dir/app.cpp.o.requires
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/requires

EditDistance/CMakeFiles/EditDistance.dir/clean:
	cd /Users/admin/Project/speech/src/EditDistance && $(CMAKE_COMMAND) -P CMakeFiles/EditDistance.dir/cmake_clean.cmake
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/clean

EditDistance/CMakeFiles/EditDistance.dir/depend:
	cd /Users/admin/Project/speech/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/admin/Project/speech/src /Users/admin/Project/speech/src/EditDistance /Users/admin/Project/speech/src /Users/admin/Project/speech/src/EditDistance /Users/admin/Project/speech/src/EditDistance/CMakeFiles/EditDistance.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : EditDistance/CMakeFiles/EditDistance.dir/depend
