cmake_minimum_required(VERSION 3.0)

enable_testing()

set (CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pg")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(maxHeap
    MaxHeap.cpp
    SegmentTreeMax.cpp
    hld.cpp
    )

add_subdirectory(tests)

add_executable(falcon
    falcon.cpp)

target_link_libraries(falcon
    maxHeap)

add_executable(speedTest
    speedTest.cpp)

target_link_libraries(speedTest
    maxHeap)
