cmake_minimum_required(VERSION 3.6.2)
#set(CMAKE_C_COMPILER /cygdrive/s/program_files/cygwin/bin/gcc.exe CACHE PATH "" FORCE)
#set(CMAKE_CXX_COMPILER /cygdrive/s/program_files/cygwin/bin/g++.exe CACHE PATH "" FORCE)
project(corewar C)
set(CMAKE_C_STANDARD 90)

add_executable(corewar_test
        src/main.c)
target_link_libraries(corewar_test
        LINK_PRIVATE ${CMAKE_SOURCE_DIR}/libft/libft.a
        )

add_executable(corewar_vm
		vm/parse_args.c
        vm/vm_main.c
        vm/vm.c
        vm/proc.c
        vm/operation.c
        vm/commands.c
        vm/loader.c
        vm/endian_conversion.c
        vm/state_writer.c
        vm/op.c)
target_link_libraries(corewar_vm
        LINK_PRIVATE ${CMAKE_SOURCE_DIR}/libft/libft.a
        )
target_include_directories(corewar_vm PUBLIC headers)

add_executable(asm
        asm/src/asm.c
        asm/src/read_input.c
        asm/src/parser.c
        asm/src/error_manager.c
        asm/src/op.c
        asm/src/parsing_header.c
        asm/src/parsing_args.c
        asm/src/parsing_oper.c
        asm/src/parsing_labels.c
        asm/src/replace_args2numbers.c
        asm/src/translator.c
        asm/src/options.c
        asm/src/cleanup.c
        asm/src/dissasembler.c)

target_include_directories(asm PUBLIC asm/include)
target_link_libraries(asm
        LINK_PRIVATE ${CMAKE_SOURCE_DIR}/libft/libft.a
        )

include_directories(libft)
