# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /datadisk0/sambashare0/coding/gitrepo/shmlib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /datadisk0/sambashare0/coding/gitrepo/shmlib/output

# Include any dependencies generated for this target.
include unit_tests/CMakeFiles/SharedMemoryTest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include unit_tests/CMakeFiles/SharedMemoryTest.dir/compiler_depend.make

# Include the progress variables for this target.
include unit_tests/CMakeFiles/SharedMemoryTest.dir/progress.make

# Include the compile flags for this target's objects.
include unit_tests/CMakeFiles/SharedMemoryTest.dir/flags.make

unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o: unit_tests/CMakeFiles/SharedMemoryTest.dir/flags.make
unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o: /datadisk0/sambashare0/coding/gitrepo/shmlib/unit_tests/shm/SharedMemoryTest.cpp
unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o: unit_tests/CMakeFiles/SharedMemoryTest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/datadisk0/sambashare0/coding/gitrepo/shmlib/output/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o"
	cd /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o -MF CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o.d -o CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o -c /datadisk0/sambashare0/coding/gitrepo/shmlib/unit_tests/shm/SharedMemoryTest.cpp

unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.i"
	cd /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /datadisk0/sambashare0/coding/gitrepo/shmlib/unit_tests/shm/SharedMemoryTest.cpp > CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.i

unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.s"
	cd /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /datadisk0/sambashare0/coding/gitrepo/shmlib/unit_tests/shm/SharedMemoryTest.cpp -o CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.s

# Object files for target SharedMemoryTest
SharedMemoryTest_OBJECTS = \
"CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o"

# External object files for target SharedMemoryTest
SharedMemoryTest_EXTERNAL_OBJECTS =

unit_tests/SharedMemoryTest: unit_tests/CMakeFiles/SharedMemoryTest.dir/shm/SharedMemoryTest.cpp.o
unit_tests/SharedMemoryTest: unit_tests/CMakeFiles/SharedMemoryTest.dir/build.make
unit_tests/SharedMemoryTest: libshmlib.so
unit_tests/SharedMemoryTest: unit_tests/CMakeFiles/SharedMemoryTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/datadisk0/sambashare0/coding/gitrepo/shmlib/output/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SharedMemoryTest"
	cd /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SharedMemoryTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unit_tests/CMakeFiles/SharedMemoryTest.dir/build: unit_tests/SharedMemoryTest
.PHONY : unit_tests/CMakeFiles/SharedMemoryTest.dir/build

unit_tests/CMakeFiles/SharedMemoryTest.dir/clean:
	cd /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests && $(CMAKE_COMMAND) -P CMakeFiles/SharedMemoryTest.dir/cmake_clean.cmake
.PHONY : unit_tests/CMakeFiles/SharedMemoryTest.dir/clean

unit_tests/CMakeFiles/SharedMemoryTest.dir/depend:
	cd /datadisk0/sambashare0/coding/gitrepo/shmlib/output && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /datadisk0/sambashare0/coding/gitrepo/shmlib /datadisk0/sambashare0/coding/gitrepo/shmlib/unit_tests /datadisk0/sambashare0/coding/gitrepo/shmlib/output /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests /datadisk0/sambashare0/coding/gitrepo/shmlib/output/unit_tests/CMakeFiles/SharedMemoryTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unit_tests/CMakeFiles/SharedMemoryTest.dir/depend
