cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

project(and-xor-or)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

add_executable(and-xor-or
    and-xor-or.cpp
    )
