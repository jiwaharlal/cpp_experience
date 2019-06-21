cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pg -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pg")

add_executable(matrix
    matrix.cpp)

find_package(GTest)
if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

add_executable(testMatrix
    matrix_test.cpp
    #matrix.cpp
    )

target_link_libraries(testMatrix
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    )

gtest_discover_tests(testMatrix)
