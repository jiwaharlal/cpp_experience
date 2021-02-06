cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

enable_testing()

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pg")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testAvl
    testAvl.cpp
    TAvlSet.hpp
    )

find_package(GTest)
if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

add_executable(testAvlSet
    test_AvlSet.cpp
    )

target_link_libraries(testAvlSet
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    )

gtest_discover_tests(testAvlSet)
