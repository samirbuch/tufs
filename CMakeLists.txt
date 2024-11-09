cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 11)

project(libtufs C)
include_directories(./lib)
add_library(libtufs
        lib/disk/disk.c
        lib/disk/disk.h
        lib/tufsio/tufsio.h
        lib/libtufs.h
        lib/tufsdef.h)

project(tufsimpl C)
include_directories(./impl)
add_executable(tufsimpl
        impl/main.c
        impl/util/util.h
        impl/commands/commands.h
        impl/commands/new.c
        impl/main.h)
target_link_libraries(tufsimpl libtufs)

find_library(READLINE_LIBRARY readline)
find_library(NCURSES_LIBRARY ncurses)
if (READLINE_LIBRARY AND NCURSES_LIBRARY)
    target_link_libraries(tufsimpl ${READLINE_LIBRARY} ${NCURSES_LIBRARY})
else ()
    message(FATAL_ERROR "Readline or ncurses library not found")
endif ()