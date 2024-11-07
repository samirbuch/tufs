cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 11)

project(libtufs C)
include_directories(./lib)
add_library(libtufs
        lib/disk.c
        lib/disk.h)

project(tufsimpl C)
include_directories(./impl)
add_executable(tufsimpl
        impl/main.c)
target_link_libraries(tufsimpl libtufs)
