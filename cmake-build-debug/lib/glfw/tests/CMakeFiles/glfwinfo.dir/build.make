# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /home/daniil/Documents/TarBalls/clion-2022.3.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/daniil/Documents/TarBalls/clion-2022.3.1/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniil/Documents/VulkanCXXLearn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug

# Include any dependencies generated for this target.
include lib/glfw/tests/CMakeFiles/glfwinfo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/glfw/tests/CMakeFiles/glfwinfo.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/glfw/tests/CMakeFiles/glfwinfo.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glfw/tests/CMakeFiles/glfwinfo.dir/flags.make

lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/flags.make
lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.o: /home/daniil/Documents/VulkanCXXLearn/lib/glfw/tests/glfwinfo.c
lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.o"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.o -MF CMakeFiles/glfwinfo.dir/glfwinfo.c.o.d -o CMakeFiles/glfwinfo.dir/glfwinfo.c.o -c /home/daniil/Documents/VulkanCXXLearn/lib/glfw/tests/glfwinfo.c

lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfwinfo.dir/glfwinfo.c.i"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/daniil/Documents/VulkanCXXLearn/lib/glfw/tests/glfwinfo.c > CMakeFiles/glfwinfo.dir/glfwinfo.c.i

lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfwinfo.dir/glfwinfo.c.s"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/daniil/Documents/VulkanCXXLearn/lib/glfw/tests/glfwinfo.c -o CMakeFiles/glfwinfo.dir/glfwinfo.c.s

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/flags.make
lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o: /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/getopt.c
lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o -MF CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o.d -o CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o -c /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/getopt.c

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfwinfo.dir/__/deps/getopt.c.i"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/getopt.c > CMakeFiles/glfwinfo.dir/__/deps/getopt.c.i

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfwinfo.dir/__/deps/getopt.c.s"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/getopt.c -o CMakeFiles/glfwinfo.dir/__/deps/getopt.c.s

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/flags.make
lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o: /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_gl.c
lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o -MF CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o.d -o CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o -c /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_gl.c

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.i"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_gl.c > CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.i

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.s"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_gl.c -o CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.s

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/flags.make
lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o: /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_vulkan.c
lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o: lib/glfw/tests/CMakeFiles/glfwinfo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o -MF CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o.d -o CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o -c /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_vulkan.c

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.i"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_vulkan.c > CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.i

lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.s"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/daniil/Documents/VulkanCXXLearn/lib/glfw/deps/glad_vulkan.c -o CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.s

# Object files for target glfwinfo
glfwinfo_OBJECTS = \
"CMakeFiles/glfwinfo.dir/glfwinfo.c.o" \
"CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o" \
"CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o" \
"CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o"

# External object files for target glfwinfo
glfwinfo_EXTERNAL_OBJECTS =

lib/glfw/tests/glfwinfo: lib/glfw/tests/CMakeFiles/glfwinfo.dir/glfwinfo.c.o
lib/glfw/tests/glfwinfo: lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/getopt.c.o
lib/glfw/tests/glfwinfo: lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_gl.c.o
lib/glfw/tests/glfwinfo: lib/glfw/tests/CMakeFiles/glfwinfo.dir/__/deps/glad_vulkan.c.o
lib/glfw/tests/glfwinfo: lib/glfw/tests/CMakeFiles/glfwinfo.dir/build.make
lib/glfw/tests/glfwinfo: lib/glfw/src/libglfw3.a
lib/glfw/tests/glfwinfo: /usr/lib/x86_64-linux-gnu/libm.so
lib/glfw/tests/glfwinfo: /usr/lib/x86_64-linux-gnu/librt.a
lib/glfw/tests/glfwinfo: /usr/lib/x86_64-linux-gnu/libm.so
lib/glfw/tests/glfwinfo: /usr/lib/x86_64-linux-gnu/libX11.so
lib/glfw/tests/glfwinfo: lib/glfw/tests/CMakeFiles/glfwinfo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable glfwinfo"
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glfwinfo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glfw/tests/CMakeFiles/glfwinfo.dir/build: lib/glfw/tests/glfwinfo
.PHONY : lib/glfw/tests/CMakeFiles/glfwinfo.dir/build

lib/glfw/tests/CMakeFiles/glfwinfo.dir/clean:
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/glfwinfo.dir/cmake_clean.cmake
.PHONY : lib/glfw/tests/CMakeFiles/glfwinfo.dir/clean

lib/glfw/tests/CMakeFiles/glfwinfo.dir/depend:
	cd /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniil/Documents/VulkanCXXLearn /home/daniil/Documents/VulkanCXXLearn/lib/glfw/tests /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests /home/daniil/Documents/VulkanCXXLearn/cmake-build-debug/lib/glfw/tests/CMakeFiles/glfwinfo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glfw/tests/CMakeFiles/glfwinfo.dir/depend

