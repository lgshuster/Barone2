# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero"

# Include any dependencies generated for this target.
include examples/client_server_c/CMakeFiles/server_node_c.dir/depend.make

# Include the progress variables for this target.
include examples/client_server_c/CMakeFiles/server_node_c.dir/progress.make

# Include the compile flags for this target's objects.
include examples/client_server_c/CMakeFiles/server_node_c.dir/flags.make

examples/client_server_c/CMakeFiles/server_node_c.dir/server_node_c.c.o: examples/client_server_c/CMakeFiles/server_node_c.dir/flags.make
examples/client_server_c/CMakeFiles/server_node_c.dir/server_node_c.c.o: examples/client_server_c/server_node_c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/client_server_c/CMakeFiles/server_node_c.dir/server_node_c.c.o"
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server_node_c.dir/server_node_c.c.o   -c "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c/server_node_c.c"

examples/client_server_c/CMakeFiles/server_node_c.dir/server_node_c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server_node_c.dir/server_node_c.c.i"
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c/server_node_c.c" > CMakeFiles/server_node_c.dir/server_node_c.c.i

examples/client_server_c/CMakeFiles/server_node_c.dir/server_node_c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server_node_c.dir/server_node_c.c.s"
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c/server_node_c.c" -o CMakeFiles/server_node_c.dir/server_node_c.c.s

# Object files for target server_node_c
server_node_c_OBJECTS = \
"CMakeFiles/server_node_c.dir/server_node_c.c.o"

# External object files for target server_node_c
server_node_c_EXTERNAL_OBJECTS =

examples/client_server_c/server_node_c: examples/client_server_c/CMakeFiles/server_node_c.dir/server_node_c.c.o
examples/client_server_c/server_node_c: examples/client_server_c/CMakeFiles/server_node_c.dir/build.make
examples/client_server_c/server_node_c: libb0.so
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libzmq.so
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_atomic.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_timer.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
examples/client_server_c/server_node_c: /usr/lib/x86_64-linux-gnu/libz.so
examples/client_server_c/server_node_c: examples/client_server_c/CMakeFiles/server_node_c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable server_node_c"
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_node_c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/client_server_c/CMakeFiles/server_node_c.dir/build: examples/client_server_c/server_node_c

.PHONY : examples/client_server_c/CMakeFiles/server_node_c.dir/build

examples/client_server_c/CMakeFiles/server_node_c.dir/clean:
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" && $(CMAKE_COMMAND) -P CMakeFiles/server_node_c.dir/cmake_clean.cmake
.PHONY : examples/client_server_c/CMakeFiles/server_node_c.dir/clean

examples/client_server_c/CMakeFiles/server_node_c.dir/depend:
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/examples/client_server_c/CMakeFiles/server_node_c.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : examples/client_server_c/CMakeFiles/server_node_c.dir/depend

