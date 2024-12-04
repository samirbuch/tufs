cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 11)

#add_compile_options(-Wall -Werror)
add_compile_options(-Wall)

project(tufs C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

include_directories(./lib)
include_directories(./impl)

# Will output libtufs.a
add_library(tufs
        lib/disk/disk.c
        lib/disk/disk.h
        lib/tufsio/tufsio.h
        lib/libtufs.h
        lib/tufsdef.h
        lib/disk/init_fs.c
        lib/disk/mount_fs.c
        lib/tufsio/fs_create.c
        lib/disk/unmount_fs.c
        lib/disk/sync_fs.c
        lib/tufsio/fs_open.c
        lib/tufsio/fs_close.c
        lib/tufsio/fs_get_filesize.c
        lib/tufsio/fs_delete.c
        lib/tufsio/fs_rename.c
        lib/tufsio/fs_read.c
        lib/tufsio/fs_write.c
        lib/tufsio/fs_lseek.c
        lib/tufsio/fs_truncate.c)
# Will link against the math library
target_link_libraries(tufs m)

option(BUILD_TUFSIMPL "Build the tufsimpl executable" ON)

if(NOT BUILD_TUFSIMPL)
    return()
endif()

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
        impl/commands/mount_disk.c
        impl/commands/unmount_disk.c
        impl/main.h
        impl/commands/create_file.c
        impl/commands/quit_repl.c
        impl/commands/shell_cmd.c
        impl/util/join.c
        impl/commands/sync_disc.c
        impl/commands/list_files.c
        impl/commands/file_info.c
        impl/commands/filesystem_info.c
        impl/commands/remove_file.c
        impl/commands/rename_file.c
        impl/commands/edit_file.c
        impl/commands/display_file_contents.c
        impl/commands/write_to_file.c
        impl/commands/list_all_commands.c
        impl/commands/copy_file.c
        impl/commands/truncate_file.c)

target_link_libraries(tufsimpl tufs)

find_library(READLINE_LIBRARY readline)
find_library(NCURSES_LIBRARY ncurses)
if (READLINE_LIBRARY AND NCURSES_LIBRARY)
    target_link_libraries(tufsimpl ${READLINE_LIBRARY} ${NCURSES_LIBRARY})
else ()
    message(FATAL_ERROR "Readline or ncurses library not found")
endif ()