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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/longlong/hhvm-gearman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/longlong/hhvm-gearman

# Include any dependencies generated for this target.
include CMakeFiles/gearman.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gearman.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gearman.dir/flags.make

CMakeFiles/gearman.dir/gearman.cpp.o: CMakeFiles/gearman.dir/flags.make
CMakeFiles/gearman.dir/gearman.cpp.o: gearman.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/longlong/hhvm-gearman/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/gearman.dir/gearman.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gearman.dir/gearman.cpp.o -c /home/longlong/hhvm-gearman/gearman.cpp

CMakeFiles/gearman.dir/gearman.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gearman.dir/gearman.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/longlong/hhvm-gearman/gearman.cpp > CMakeFiles/gearman.dir/gearman.cpp.i

CMakeFiles/gearman.dir/gearman.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gearman.dir/gearman.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/longlong/hhvm-gearman/gearman.cpp -o CMakeFiles/gearman.dir/gearman.cpp.s

CMakeFiles/gearman.dir/gearman.cpp.o.requires:
.PHONY : CMakeFiles/gearman.dir/gearman.cpp.o.requires

CMakeFiles/gearman.dir/gearman.cpp.o.provides: CMakeFiles/gearman.dir/gearman.cpp.o.requires
	$(MAKE) -f CMakeFiles/gearman.dir/build.make CMakeFiles/gearman.dir/gearman.cpp.o.provides.build
.PHONY : CMakeFiles/gearman.dir/gearman.cpp.o.provides

CMakeFiles/gearman.dir/gearman.cpp.o.provides.build: CMakeFiles/gearman.dir/gearman.cpp.o

# Object files for target gearman
gearman_OBJECTS = \
"CMakeFiles/gearman.dir/gearman.cpp.o"

# External object files for target gearman
gearman_EXTERNAL_OBJECTS =

gearman.so: CMakeFiles/gearman.dir/gearman.cpp.o
gearman.so: CMakeFiles/gearman.dir/build.make
gearman.so: /usr/lib/x86_64-linux-gnu/libgearman.so
gearman.so: ext_gearman.php
gearman.so: CMakeFiles/gearman.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library gearman.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gearman.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Embedding php in gearman"
	objcopy --add-section ext.5500949e412e=ext_gearman.php gearman.so

# Rule to build all files generated by this target.
CMakeFiles/gearman.dir/build: gearman.so
.PHONY : CMakeFiles/gearman.dir/build

CMakeFiles/gearman.dir/requires: CMakeFiles/gearman.dir/gearman.cpp.o.requires
.PHONY : CMakeFiles/gearman.dir/requires

CMakeFiles/gearman.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gearman.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gearman.dir/clean

CMakeFiles/gearman.dir/depend:
	cd /home/longlong/hhvm-gearman && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/longlong/hhvm-gearman /home/longlong/hhvm-gearman /home/longlong/hhvm-gearman /home/longlong/hhvm-gearman /home/longlong/hhvm-gearman/CMakeFiles/gearman.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gearman.dir/depend

