cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pg")

add_library(common STATIC
    graph.cpp
    )

target_include_directories(common
    PUBLIC ${PROJECT_SOURCE_DIR}
    )

#target_link_libraries(${GEOMETRY_LINESTRING}
    #PUBLIC avfw_numeric
    #PUBLIC preprocessor
    #PUBLIC algorithms_geo_utils
    #)
