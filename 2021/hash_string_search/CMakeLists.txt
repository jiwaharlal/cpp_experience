cmake_minimum_required(VERSION 3.0)

set(PROJECT hs-search LANGUAGES CXX)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(hs-search
    hash_string_search.cpp
    )

target_link_libraries(hs-search
    Boost::program_options
    )
