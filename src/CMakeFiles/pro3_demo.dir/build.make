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
include CMakeFiles/pro3_demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pro3_demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pro3_demo.dir/flags.make

CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o: CMakeFiles/pro3_demo.dir/flags.make
CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o: pro3_demo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/admin/Project/speech/src/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o -c /Users/admin/Project/speech/src/pro3_demo.cpp

CMakeFiles/pro3_demo.dir/pro3_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pro3_demo.dir/pro3_demo.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/admin/Project/speech/src/pro3_demo.cpp > CMakeFiles/pro3_demo.dir/pro3_demo.cpp.i

CMakeFiles/pro3_demo.dir/pro3_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pro3_demo.dir/pro3_demo.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/admin/Project/speech/src/pro3_demo.cpp -o CMakeFiles/pro3_demo.dir/pro3_demo.cpp.s

CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.requires:
.PHONY : CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.requires

CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.provides: CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.requires
	$(MAKE) -f CMakeFiles/pro3_demo.dir/build.make CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.provides.build
.PHONY : CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.provides

CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.provides.build: CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o

# Object files for target pro3_demo
pro3_demo_OBJECTS = \
"CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o"

# External object files for target pro3_demo
pro3_demo_EXTERNAL_OBJECTS =

pro3_demo: CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o
pro3_demo: CMakeFiles/pro3_demo.dir/build.make
pro3_demo: Configure/libConfigure.a
pro3_demo: EditDistance/libEditDistance.a
pro3_demo: ThreadPool/libThreadPool.a
pro3_demo: CMakeFiles/pro3_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable pro3_demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pro3_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pro3_demo.dir/build: pro3_demo
.PHONY : CMakeFiles/pro3_demo.dir/build

CMakeFiles/pro3_demo.dir/requires: CMakeFiles/pro3_demo.dir/pro3_demo.cpp.o.requires
.PHONY : CMakeFiles/pro3_demo.dir/requires

CMakeFiles/pro3_demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pro3_demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pro3_demo.dir/clean

CMakeFiles/pro3_demo.dir/depend:
	cd /Users/admin/Project/speech/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/admin/Project/speech/src /Users/admin/Project/speech/src /Users/admin/Project/speech/src /Users/admin/Project/speech/src /Users/admin/Project/speech/src/CMakeFiles/pro3_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pro3_demo.dir/depend
