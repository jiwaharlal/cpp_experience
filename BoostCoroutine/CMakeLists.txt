cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_package(Boost COMPONENTS program_options filesystem log context REQUIRED)

add_executable(boostCoroutine
    boost_coroutine.cpp
    )

#Static start
#set_target_properties(boostCoroutine PROPERTIES LINK_SEARCH_START_STATIC 1)
#set_target_properties(boostCoroutine PROPERTIES LINK_SEARCH_END_STATIC 1)
#Static Libs

#Set Linker flags
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

target_link_libraries(boostCoroutine
    PRIVATE Boost::context
    #boost_coroutine
    #boost_system
    )
