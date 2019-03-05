cmake_minimum_required(VERSION 3.0)

enable_testing()

set (CMAKE_CXX_STANDARD 14)

SET(GCC_COVERAGE_COMPILE_FLAGS "-g -fprofile-arcs -ftest-coverage --coverage")
#SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_library(ring STATIC
    src/Ring.cpp
    )

target_link_libraries(ring
    PRIVATE gcov
    )

add_executable(matrix-rotate
    src/matrix-rotate.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)

target_link_libraries(matrix-rotate
    PRIVATE ring
    )
