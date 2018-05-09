cmake_minimum_required(VERSION 3.0)

find_package(Boost COMPONENTS program_options filesystem log REQUIRED)

add_executable(boostLog
    src/boost_log.cpp)

target_link_libraries(boostLog
    Boost::log)
