# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kgaft/Documents/KGAFTEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kgaft/Documents/KGAFTEngine/build

# Include any dependencies generated for this target.
include CMakeFiles/KGAFTEngine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/KGAFTEngine.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/KGAFTEngine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/KGAFTEngine.dir/flags.make

CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o: CMakeFiles/KGAFTEngine.dir/flags.make
CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o: /home/kgaft/Documents/KGAFTEngine/src/cxx/External/stb_image.cpp
CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o: CMakeFiles/KGAFTEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kgaft/Documents/KGAFTEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o -MF CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o.d -o CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o -c /home/kgaft/Documents/KGAFTEngine/src/cxx/External/stb_image.cpp

CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kgaft/Documents/KGAFTEngine/src/cxx/External/stb_image.cpp > CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.i

CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kgaft/Documents/KGAFTEngine/src/cxx/External/stb_image.cpp -o CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.s

CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o: CMakeFiles/KGAFTEngine.dir/flags.make
CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o: /home/kgaft/Documents/KGAFTEngine/src/cxx/main.cpp
CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o: CMakeFiles/KGAFTEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kgaft/Documents/KGAFTEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o -MF CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o.d -o CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o -c /home/kgaft/Documents/KGAFTEngine/src/cxx/main.cpp

CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kgaft/Documents/KGAFTEngine/src/cxx/main.cpp > CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.i

CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kgaft/Documents/KGAFTEngine/src/cxx/main.cpp -o CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.s

# Object files for target KGAFTEngine
KGAFTEngine_OBJECTS = \
"CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o" \
"CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o"

# External object files for target KGAFTEngine
KGAFTEngine_EXTERNAL_OBJECTS =

KGAFTEngine: CMakeFiles/KGAFTEngine.dir/src/cxx/External/stb_image.cpp.o
KGAFTEngine: CMakeFiles/KGAFTEngine.dir/src/cxx/main.cpp.o
KGAFTEngine: CMakeFiles/KGAFTEngine.dir/build.make
KGAFTEngine: src/modules/VulkanContext/libVulkanContext.a
KGAFTEngine: lib/glfw/src/libglfw3.a
KGAFTEngine: /usr/lib/librt.a
KGAFTEngine: /usr/lib/libm.so
KGAFTEngine: CMakeFiles/KGAFTEngine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kgaft/Documents/KGAFTEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable KGAFTEngine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/KGAFTEngine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/KGAFTEngine.dir/build: KGAFTEngine
.PHONY : CMakeFiles/KGAFTEngine.dir/build

CMakeFiles/KGAFTEngine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/KGAFTEngine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/KGAFTEngine.dir/clean

CMakeFiles/KGAFTEngine.dir/depend:
	cd /home/kgaft/Documents/KGAFTEngine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kgaft/Documents/KGAFTEngine /home/kgaft/Documents/KGAFTEngine /home/kgaft/Documents/KGAFTEngine/build /home/kgaft/Documents/KGAFTEngine/build /home/kgaft/Documents/KGAFTEngine/build/CMakeFiles/KGAFTEngine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/KGAFTEngine.dir/depend
