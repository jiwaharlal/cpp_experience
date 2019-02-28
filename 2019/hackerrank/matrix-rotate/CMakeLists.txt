cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

add_library(ring STATIC
    src/Ring.cpp)

add_executable(matrix-rotate
    src/matrix-rotate.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)

target_link_libraries(matrix-rotate
    PRIVATE ring
    )
