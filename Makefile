# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = "/Users/fff/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/182.3684.76/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/fff/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/182.3684.76/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/fff/Documents/Git/dr_mauro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/fff/Documents/Git/dr_mauro

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"/Users/fff/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/182.3684.76/CLion.app/Contents/bin/cmake/mac/bin/cmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	"/Users/fff/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/182.3684.76/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/fff/Documents/Git/dr_mauro/CMakeFiles /Users/fff/Documents/Git/dr_mauro/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/fff/Documents/Git/dr_mauro/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named dr_mauro

# Build rule for target.
dr_mauro: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 dr_mauro
.PHONY : dr_mauro

# fast build rule for target.
dr_mauro/fast:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/build
.PHONY : dr_mauro/fast

drmauro.o: drmauro.c.o

.PHONY : drmauro.o

# target to build an object file
drmauro.c.o:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/drmauro.c.o
.PHONY : drmauro.c.o

drmauro.i: drmauro.c.i

.PHONY : drmauro.i

# target to preprocess a source file
drmauro.c.i:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/drmauro.c.i
.PHONY : drmauro.c.i

drmauro.s: drmauro.c.s

.PHONY : drmauro.s

# target to generate assembly for a file
drmauro.c.s:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/drmauro.c.s
.PHONY : drmauro.c.s

drmauro_main.o: drmauro_main.c.o

.PHONY : drmauro_main.o

# target to build an object file
drmauro_main.c.o:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/drmauro_main.c.o
.PHONY : drmauro_main.c.o

drmauro_main.i: drmauro_main.c.i

.PHONY : drmauro_main.i

# target to preprocess a source file
drmauro_main.c.i:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/drmauro_main.c.i
.PHONY : drmauro_main.c.i

drmauro_main.s: drmauro_main.c.s

.PHONY : drmauro_main.s

# target to generate assembly for a file
drmauro_main.c.s:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/drmauro_main.c.s
.PHONY : drmauro_main.c.s

game.o: game.c.o

.PHONY : game.o

# target to build an object file
game.c.o:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/game.c.o
.PHONY : game.c.o

game.i: game.c.i

.PHONY : game.i

# target to preprocess a source file
game.c.i:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/game.c.i
.PHONY : game.c.i

game.s: game.c.s

.PHONY : game.s

# target to generate assembly for a file
game.c.s:
	$(MAKE) -f CMakeFiles/dr_mauro.dir/build.make CMakeFiles/dr_mauro.dir/game.c.s
.PHONY : game.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... dr_mauro"
	@echo "... drmauro.o"
	@echo "... drmauro.i"
	@echo "... drmauro.s"
	@echo "... drmauro_main.o"
	@echo "... drmauro_main.i"
	@echo "... drmauro_main.s"
	@echo "... game.o"
	@echo "... game.i"
	@echo "... game.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

