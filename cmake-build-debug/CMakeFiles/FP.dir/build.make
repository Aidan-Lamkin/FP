# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aidan/Documents/CSCI_441/FP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aidan/Documents/CSCI_441/FP/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FP.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FP.dir/flags.make

CMakeFiles/FP.dir/main.cpp.o: CMakeFiles/FP.dir/flags.make
CMakeFiles/FP.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FP.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/main.cpp.o -c /Users/aidan/Documents/CSCI_441/FP/main.cpp

CMakeFiles/FP.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/FP/main.cpp > CMakeFiles/FP.dir/main.cpp.i

CMakeFiles/FP.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/FP/main.cpp -o CMakeFiles/FP.dir/main.cpp.s

CMakeFiles/FP.dir/FPEngine.cpp.o: CMakeFiles/FP.dir/flags.make
CMakeFiles/FP.dir/FPEngine.cpp.o: ../FPEngine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FP.dir/FPEngine.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/FPEngine.cpp.o -c /Users/aidan/Documents/CSCI_441/FP/FPEngine.cpp

CMakeFiles/FP.dir/FPEngine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/FPEngine.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/FP/FPEngine.cpp > CMakeFiles/FP.dir/FPEngine.cpp.i

CMakeFiles/FP.dir/FPEngine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/FPEngine.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/FP/FPEngine.cpp -o CMakeFiles/FP.dir/FPEngine.cpp.s

CMakeFiles/FP.dir/Player.cpp.o: CMakeFiles/FP.dir/flags.make
CMakeFiles/FP.dir/Player.cpp.o: ../Player.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/FP.dir/Player.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/Player.cpp.o -c /Users/aidan/Documents/CSCI_441/FP/Player.cpp

CMakeFiles/FP.dir/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/Player.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/FP/Player.cpp > CMakeFiles/FP.dir/Player.cpp.i

CMakeFiles/FP.dir/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/Player.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/FP/Player.cpp -o CMakeFiles/FP.dir/Player.cpp.s

CMakeFiles/FP.dir/Enemy.cpp.o: CMakeFiles/FP.dir/flags.make
CMakeFiles/FP.dir/Enemy.cpp.o: ../Enemy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/FP.dir/Enemy.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/Enemy.cpp.o -c /Users/aidan/Documents/CSCI_441/FP/Enemy.cpp

CMakeFiles/FP.dir/Enemy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/Enemy.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/FP/Enemy.cpp > CMakeFiles/FP.dir/Enemy.cpp.i

CMakeFiles/FP.dir/Enemy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/Enemy.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/FP/Enemy.cpp -o CMakeFiles/FP.dir/Enemy.cpp.s

CMakeFiles/FP.dir/Bullet.cpp.o: CMakeFiles/FP.dir/flags.make
CMakeFiles/FP.dir/Bullet.cpp.o: ../Bullet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/FP.dir/Bullet.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/Bullet.cpp.o -c /Users/aidan/Documents/CSCI_441/FP/Bullet.cpp

CMakeFiles/FP.dir/Bullet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/Bullet.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/FP/Bullet.cpp > CMakeFiles/FP.dir/Bullet.cpp.i

CMakeFiles/FP.dir/Bullet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/Bullet.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/FP/Bullet.cpp -o CMakeFiles/FP.dir/Bullet.cpp.s

# Object files for target FP
FP_OBJECTS = \
"CMakeFiles/FP.dir/main.cpp.o" \
"CMakeFiles/FP.dir/FPEngine.cpp.o" \
"CMakeFiles/FP.dir/Player.cpp.o" \
"CMakeFiles/FP.dir/Enemy.cpp.o" \
"CMakeFiles/FP.dir/Bullet.cpp.o"

# External object files for target FP
FP_EXTERNAL_OBJECTS =

FP: CMakeFiles/FP.dir/main.cpp.o
FP: CMakeFiles/FP.dir/FPEngine.cpp.o
FP: CMakeFiles/FP.dir/Player.cpp.o
FP: CMakeFiles/FP.dir/Enemy.cpp.o
FP: CMakeFiles/FP.dir/Bullet.cpp.o
FP: CMakeFiles/FP.dir/build.make
FP: CMakeFiles/FP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable FP"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FP.dir/build: FP

.PHONY : CMakeFiles/FP.dir/build

CMakeFiles/FP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FP.dir/clean

CMakeFiles/FP.dir/depend:
	cd /Users/aidan/Documents/CSCI_441/FP/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/aidan/Documents/CSCI_441/FP /Users/aidan/Documents/CSCI_441/FP /Users/aidan/Documents/CSCI_441/FP/cmake-build-debug /Users/aidan/Documents/CSCI_441/FP/cmake-build-debug /Users/aidan/Documents/CSCI_441/FP/cmake-build-debug/CMakeFiles/FP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FP.dir/depend

