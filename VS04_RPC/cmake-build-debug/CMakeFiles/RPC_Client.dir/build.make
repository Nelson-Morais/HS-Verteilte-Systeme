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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ladmin/CLionProjects/VS04_RPC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/RPC_Client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RPC_Client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RPC_Client.dir/flags.make

CMakeFiles/RPC_Client.dir/client/client.c.o: CMakeFiles/RPC_Client.dir/flags.make
CMakeFiles/RPC_Client.dir/client/client.c.o: ../client/client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/RPC_Client.dir/client/client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RPC_Client.dir/client/client.c.o   -c /home/ladmin/CLionProjects/VS04_RPC/client/client.c

CMakeFiles/RPC_Client.dir/client/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RPC_Client.dir/client/client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ladmin/CLionProjects/VS04_RPC/client/client.c > CMakeFiles/RPC_Client.dir/client/client.c.i

CMakeFiles/RPC_Client.dir/client/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RPC_Client.dir/client/client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ladmin/CLionProjects/VS04_RPC/client/client.c -o CMakeFiles/RPC_Client.dir/client/client.c.s

CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.o: CMakeFiles/RPC_Client.dir/flags.make
CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.o: ../client/RPC_Protocol_files/pub_sub_clnt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.o   -c /home/ladmin/CLionProjects/VS04_RPC/client/RPC_Protocol_files/pub_sub_clnt.c

CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ladmin/CLionProjects/VS04_RPC/client/RPC_Protocol_files/pub_sub_clnt.c > CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.i

CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ladmin/CLionProjects/VS04_RPC/client/RPC_Protocol_files/pub_sub_clnt.c -o CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.s

CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.o: CMakeFiles/RPC_Client.dir/flags.make
CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.o: ../client/RPC_Protocol_files/pub_sub_xdr.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.o   -c /home/ladmin/CLionProjects/VS04_RPC/client/RPC_Protocol_files/pub_sub_xdr.c

CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ladmin/CLionProjects/VS04_RPC/client/RPC_Protocol_files/pub_sub_xdr.c > CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.i

CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ladmin/CLionProjects/VS04_RPC/client/RPC_Protocol_files/pub_sub_xdr.c -o CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.s

# Object files for target RPC_Client
RPC_Client_OBJECTS = \
"CMakeFiles/RPC_Client.dir/client/client.c.o" \
"CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.o" \
"CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.o"

# External object files for target RPC_Client
RPC_Client_EXTERNAL_OBJECTS =

RPC_Client: CMakeFiles/RPC_Client.dir/client/client.c.o
RPC_Client: CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_clnt.c.o
RPC_Client: CMakeFiles/RPC_Client.dir/client/RPC_Protocol_files/pub_sub_xdr.c.o
RPC_Client: CMakeFiles/RPC_Client.dir/build.make
RPC_Client: CMakeFiles/RPC_Client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable RPC_Client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RPC_Client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RPC_Client.dir/build: RPC_Client

.PHONY : CMakeFiles/RPC_Client.dir/build

CMakeFiles/RPC_Client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RPC_Client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RPC_Client.dir/clean

CMakeFiles/RPC_Client.dir/depend:
	cd /home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ladmin/CLionProjects/VS04_RPC /home/ladmin/CLionProjects/VS04_RPC /home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug /home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug /home/ladmin/CLionProjects/VS04_RPC/cmake-build-debug/CMakeFiles/RPC_Client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RPC_Client.dir/depend

