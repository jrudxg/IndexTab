cmake_minimum_required(VERSION 3.14)
project(quickjs C)

set(CMAKE_C_STANDARD 11)
add_definitions(-DDISABLE_ATOMICS)


add_library(quickjs STATIC quickjs-amalgam.c)

target_include_directories(quickjs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Auf Windows keine math library nötig, nur unter Linux/Mac
if(UNIX)
    target_link_libraries(quickjs PUBLIC m)
endif()
