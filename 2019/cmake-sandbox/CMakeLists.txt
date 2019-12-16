cmake_minimum_required(VERSION 3.15)

project(cmake_sandbox)
add_definitions(-DUSE_GUI=1)
#set(USE_GUI 0)

if (NOT USE_GUI)
    message(STATUS "Build without GUI")
endif()

message("USE_GUI : ${USE_GUI}")

add_executable(app
    app.cpp
    )
