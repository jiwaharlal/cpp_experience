cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_library(p-base STATIC printer-base.cpp)

add_library(p1 SHARED printer.cpp)
target_link_libraries(p1 p-base)

add_library(p2 SHARED printer2.cpp)
target_link_libraries(p2 p-base)

add_executable(static-in-shared
    static-in-shared.cpp)

target_link_libraries(static-in-shared
    p1
    p-base
    boost_system
    boost_filesystem
    dl
    )
