# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/home/Desktop/programs/random_projects/gol

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/home/Desktop/programs/random_projects/gol/build

# Include any dependencies generated for this target.
include CMakeFiles/gol.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gol.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gol.dir/flags.make

CMakeFiles/gol.dir/gol.cpp.o: CMakeFiles/gol.dir/flags.make
CMakeFiles/gol.dir/gol.cpp.o: /Users/home/Desktop/programs/random_projects/gol/gol.cpp
CMakeFiles/gol.dir/gol.cpp.o: CMakeFiles/gol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/home/Desktop/programs/random_projects/gol/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gol.dir/gol.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gol.dir/gol.cpp.o -MF CMakeFiles/gol.dir/gol.cpp.o.d -o CMakeFiles/gol.dir/gol.cpp.o -c /Users/home/Desktop/programs/random_projects/gol/gol.cpp

CMakeFiles/gol.dir/gol.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/gol.dir/gol.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/home/Desktop/programs/random_projects/gol/gol.cpp > CMakeFiles/gol.dir/gol.cpp.i

CMakeFiles/gol.dir/gol.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/gol.dir/gol.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/home/Desktop/programs/random_projects/gol/gol.cpp -o CMakeFiles/gol.dir/gol.cpp.s

# Object files for target gol
gol_OBJECTS = \
"CMakeFiles/gol.dir/gol.cpp.o"

# External object files for target gol
gol_EXTERNAL_OBJECTS =

gol: CMakeFiles/gol.dir/gol.cpp.o
gol: CMakeFiles/gol.dir/build.make
gol: /opt/homebrew/Cellar/sdl2/2.30.0/lib/libSDL2.dylib
gol: CMakeFiles/gol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/home/Desktop/programs/random_projects/gol/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gol.dir/build: gol
.PHONY : CMakeFiles/gol.dir/build

CMakeFiles/gol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gol.dir/clean

CMakeFiles/gol.dir/depend:
	cd /Users/home/Desktop/programs/random_projects/gol/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/home/Desktop/programs/random_projects/gol /Users/home/Desktop/programs/random_projects/gol /Users/home/Desktop/programs/random_projects/gol/build /Users/home/Desktop/programs/random_projects/gol/build /Users/home/Desktop/programs/random_projects/gol/build/CMakeFiles/gol.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/gol.dir/depend
