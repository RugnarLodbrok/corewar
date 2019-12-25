cmake_minimum_required(VERSION 3.6.2)
#set(CMAKE_C_COMPILER /cygdrive/s/program_files/cygwin/bin/gcc.exe CACHE PATH "" FORCE)
#set(CMAKE_CXX_COMPILER /cygdrive/s/program_files/cygwin/bin/g++.exe CACHE PATH "" FORCE)
project(corewar C)
set(CMAKE_C_STANDARD 90)
include_directories(.)
include_directories(headers)
include_directories(libft)

add_executable(corewar_test
        src/main.c)
target_link_libraries(corewar_test
        LINK_PRIVATE ${CMAKE_SOURCE_DIR}/libft/libft.a
        )

add_executable(corewar_vm
        vm/vm.c)
target_link_libraries(corewar_vm
        LINK_PRIVATE ${CMAKE_SOURCE_DIR}/libft/libft.a
        )
