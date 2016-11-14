cmake_minimum_required( VERSION 2.8 )

set(ACTION_NAME action)

set(projectName action)
project(${projectName})

INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )

#file(GLOB_RECURSE SRC "*.cpp")
set (SRC
   ActionMain.cpp
   )

ADD_EXECUTABLE(${projectName} ${SRC})

target_compile_features(${projectName} PRIVATE cxx_range_for)
