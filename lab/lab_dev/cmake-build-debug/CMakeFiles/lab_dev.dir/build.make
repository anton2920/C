# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /home/anton/Загрузки/clion-2017.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/anton/Загрузки/clion-2017.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anton/C/lab/lab_dev

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anton/C/lab/lab_dev/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lab_dev.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab_dev.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab_dev.dir/flags.make

CMakeFiles/lab_dev.dir/main.c.o: CMakeFiles/lab_dev.dir/flags.make
CMakeFiles/lab_dev.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anton/C/lab/lab_dev/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lab_dev.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lab_dev.dir/main.c.o   -c /home/anton/C/lab/lab_dev/main.c

CMakeFiles/lab_dev.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab_dev.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/anton/C/lab/lab_dev/main.c > CMakeFiles/lab_dev.dir/main.c.i

CMakeFiles/lab_dev.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab_dev.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/anton/C/lab/lab_dev/main.c -o CMakeFiles/lab_dev.dir/main.c.s

CMakeFiles/lab_dev.dir/main.c.o.requires:

.PHONY : CMakeFiles/lab_dev.dir/main.c.o.requires

CMakeFiles/lab_dev.dir/main.c.o.provides: CMakeFiles/lab_dev.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/lab_dev.dir/build.make CMakeFiles/lab_dev.dir/main.c.o.provides.build
.PHONY : CMakeFiles/lab_dev.dir/main.c.o.provides

CMakeFiles/lab_dev.dir/main.c.o.provides.build: CMakeFiles/lab_dev.dir/main.c.o


CMakeFiles/lab_dev.dir/input/input.c.o: CMakeFiles/lab_dev.dir/flags.make
CMakeFiles/lab_dev.dir/input/input.c.o: ../input/input.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anton/C/lab/lab_dev/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lab_dev.dir/input/input.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lab_dev.dir/input/input.c.o   -c /home/anton/C/lab/lab_dev/input/input.c

CMakeFiles/lab_dev.dir/input/input.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab_dev.dir/input/input.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/anton/C/lab/lab_dev/input/input.c > CMakeFiles/lab_dev.dir/input/input.c.i

CMakeFiles/lab_dev.dir/input/input.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab_dev.dir/input/input.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/anton/C/lab/lab_dev/input/input.c -o CMakeFiles/lab_dev.dir/input/input.c.s

CMakeFiles/lab_dev.dir/input/input.c.o.requires:

.PHONY : CMakeFiles/lab_dev.dir/input/input.c.o.requires

CMakeFiles/lab_dev.dir/input/input.c.o.provides: CMakeFiles/lab_dev.dir/input/input.c.o.requires
	$(MAKE) -f CMakeFiles/lab_dev.dir/build.make CMakeFiles/lab_dev.dir/input/input.c.o.provides.build
.PHONY : CMakeFiles/lab_dev.dir/input/input.c.o.provides

CMakeFiles/lab_dev.dir/input/input.c.o.provides.build: CMakeFiles/lab_dev.dir/input/input.c.o


# Object files for target lab_dev
lab_dev_OBJECTS = \
"CMakeFiles/lab_dev.dir/main.c.o" \
"CMakeFiles/lab_dev.dir/input/input.c.o"

# External object files for target lab_dev
lab_dev_EXTERNAL_OBJECTS =

lab_dev: CMakeFiles/lab_dev.dir/main.c.o
lab_dev: CMakeFiles/lab_dev.dir/input/input.c.o
lab_dev: CMakeFiles/lab_dev.dir/build.make
lab_dev: /usr/lib/x86_64-linux-gnu/libSDL2main.a
lab_dev: /usr/lib/x86_64-linux-gnu/libSDL2.so
lab_dev: CMakeFiles/lab_dev.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anton/C/lab/lab_dev/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable lab_dev"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab_dev.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab_dev.dir/build: lab_dev

.PHONY : CMakeFiles/lab_dev.dir/build

CMakeFiles/lab_dev.dir/requires: CMakeFiles/lab_dev.dir/main.c.o.requires
CMakeFiles/lab_dev.dir/requires: CMakeFiles/lab_dev.dir/input/input.c.o.requires

.PHONY : CMakeFiles/lab_dev.dir/requires

CMakeFiles/lab_dev.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab_dev.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab_dev.dir/clean

CMakeFiles/lab_dev.dir/depend:
	cd /home/anton/C/lab/lab_dev/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anton/C/lab/lab_dev /home/anton/C/lab/lab_dev /home/anton/C/lab/lab_dev/cmake-build-debug /home/anton/C/lab/lab_dev/cmake-build-debug /home/anton/C/lab/lab_dev/cmake-build-debug/CMakeFiles/lab_dev.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab_dev.dir/depend

