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
include CMakeFiles/b0_graph_monitor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/b0_graph_monitor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/b0_graph_monitor.dir/flags.make

CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.o: CMakeFiles/b0_graph_monitor.dir/flags.make
CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.o: src/b0_graph_monitor/graph_monitor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.o -c "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/src/b0_graph_monitor/graph_monitor.cpp"

CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/src/b0_graph_monitor/graph_monitor.cpp" > CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.i

CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/src/b0_graph_monitor/graph_monitor.cpp" -o CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.s

# Object files for target b0_graph_monitor
b0_graph_monitor_OBJECTS = \
"CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.o"

# External object files for target b0_graph_monitor
b0_graph_monitor_EXTERNAL_OBJECTS =

b0_graph_monitor: CMakeFiles/b0_graph_monitor.dir/src/b0_graph_monitor/graph_monitor.cpp.o
b0_graph_monitor: CMakeFiles/b0_graph_monitor.dir/build.make
b0_graph_monitor: libb0.so
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libzmq.so
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_atomic.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_timer.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
b0_graph_monitor: /usr/lib/x86_64-linux-gnu/libz.so
b0_graph_monitor: CMakeFiles/b0_graph_monitor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable b0_graph_monitor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/b0_graph_monitor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/b0_graph_monitor.dir/build: b0_graph_monitor

.PHONY : CMakeFiles/b0_graph_monitor.dir/build

CMakeFiles/b0_graph_monitor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/b0_graph_monitor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/b0_graph_monitor.dir/clean

CMakeFiles/b0_graph_monitor.dir/depend:
	cd "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero" "/mnt/c/Program Files/CoppeliaRobotics/CoppeliaSimEdu/programming/bluezero/CMakeFiles/b0_graph_monitor.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/b0_graph_monitor.dir/depend

