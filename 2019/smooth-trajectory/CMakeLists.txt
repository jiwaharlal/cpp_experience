cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g")

add_executable(smooth
    smooth-trajectory.cpp)

target_link_libraries(smooth
    PRIVATE armadillo
    )
