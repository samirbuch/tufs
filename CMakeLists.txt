cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 11)

project(libtufs C)
include_directories(./lib)
add_library(libtufs
        lib/disk/disk.c
        lib/disk/disk.h
        lib/tufsio/tufsio.h
        lib/libtufs.h
        lib/tufsdef.h
        lib/disk/init_fs.c
        lib/disk/mount_fs.c
        lib/tufsio/fs_create.c)

project(tufsimpl C)
include_directories(./impl)
add_executable(tufsimpl
        impl/main.c
        impl/util/util.h
        impl/commands/commands.h
        impl/commands/new_disk.c
        impl/main.h
        impl/util/index_of.c
        impl/util/parse.c
        impl/util/logging/logging.h
        impl/util/logging/logging.c
        impl/commands/mount_disk.c)
target_link_libraries(tufsimpl libtufs)

find_library(READLINE_LIBRARY readline)
find_library(NCURSES_LIBRARY ncurses)
if (READLINE_LIBRARY AND NCURSES_LIBRARY)
    target_link_libraries(tufsimpl ${READLINE_LIBRARY} ${NCURSES_LIBRARY})
else ()
    message(FATAL_ERROR "Readline or ncurses library not found")
endif ()