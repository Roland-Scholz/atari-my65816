cmake_minimum_required(VERSION 3.10.0)
project(my65816 VERSION 0.1.0 LANGUAGES C)

include_directories(./)
add_executable(my65816 decoder.c io.c main.c ram.c rom.c lib65816/cpu.c lib65816/cpuevent.c lib65816/debugger.c lib65816/dispatch.c lib65816/opcodes1.c lib65816/opcodes2.c lib65816/opcodes3.c lib65816/opcodes4.c lib65816/opcodes5.c lib65816/table.c)

include(CTest)
enable_testing()

