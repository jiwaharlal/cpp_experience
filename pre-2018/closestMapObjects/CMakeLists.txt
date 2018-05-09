cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

add_executable(closestObject
    closestObject.cpp
    )

target_link_libraries(closestObject
    PRIVATE bz2
    PRIVATE expat
    PRIVATE z
    PRIVATE pthread
    )
