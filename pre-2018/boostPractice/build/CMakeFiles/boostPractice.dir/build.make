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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maxim/projects/cpp_experience/boostPractice

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maxim/projects/cpp_experience/boostPractice/build

# Include any dependencies generated for this target.
include CMakeFiles/boostPractice.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/boostPractice.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/boostPractice.dir/flags.make

CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o: CMakeFiles/boostPractice.dir/flags.make
CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o: ../src/practiceMain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxim/projects/cpp_experience/boostPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o -c /home/maxim/projects/cpp_experience/boostPractice/src/practiceMain.cpp

CMakeFiles/boostPractice.dir/src/practiceMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boostPractice.dir/src/practiceMain.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxim/projects/cpp_experience/boostPractice/src/practiceMain.cpp > CMakeFiles/boostPractice.dir/src/practiceMain.cpp.i

CMakeFiles/boostPractice.dir/src/practiceMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boostPractice.dir/src/practiceMain.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxim/projects/cpp_experience/boostPractice/src/practiceMain.cpp -o CMakeFiles/boostPractice.dir/src/practiceMain.cpp.s

CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.requires:

.PHONY : CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.requires

CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.provides: CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.requires
	$(MAKE) -f CMakeFiles/boostPractice.dir/build.make CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.provides.build
.PHONY : CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.provides

CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.provides.build: CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o


# Object files for target boostPractice
boostPractice_OBJECTS = \
"CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o"

# External object files for target boostPractice
boostPractice_EXTERNAL_OBJECTS =

boostPractice: CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o
boostPractice: CMakeFiles/boostPractice.dir/build.make
boostPractice: /usr/local/lib/libboost_filesystem.so
boostPractice: /usr/local/lib/libboost_program_options.so
boostPractice: /usr/local/lib/libboost_thread.so
boostPractice: /usr/local/lib/libboost_system.so
boostPractice: /usr/local/lib/libboost_chrono.so
boostPractice: /usr/local/lib/libboost_coroutine.so
boostPractice: /usr/local/lib/libboost_context.so
boostPractice: /usr/local/lib/libboost_date_time.so
boostPractice: /usr/local/lib/libboost_atomic.so
boostPractice: CMakeFiles/boostPractice.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maxim/projects/cpp_experience/boostPractice/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable boostPractice"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/boostPractice.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/boostPractice.dir/build: boostPractice

.PHONY : CMakeFiles/boostPractice.dir/build

CMakeFiles/boostPractice.dir/requires: CMakeFiles/boostPractice.dir/src/practiceMain.cpp.o.requires

.PHONY : CMakeFiles/boostPractice.dir/requires

CMakeFiles/boostPractice.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/boostPractice.dir/cmake_clean.cmake
.PHONY : CMakeFiles/boostPractice.dir/clean

CMakeFiles/boostPractice.dir/depend:
	cd /home/maxim/projects/cpp_experience/boostPractice/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxim/projects/cpp_experience/boostPractice /home/maxim/projects/cpp_experience/boostPractice /home/maxim/projects/cpp_experience/boostPractice/build /home/maxim/projects/cpp_experience/boostPractice/build /home/maxim/projects/cpp_experience/boostPractice/build/CMakeFiles/boostPractice.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/boostPractice.dir/depend

