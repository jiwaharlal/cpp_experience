cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")

set(projectName actor)
project(${projectName})

find_package(Boost COMPONENTS program_options thread system chrono REQUIRED)

#FILE (GLOB SRC *.cpp)
set (SRC actorMain.cpp)

add_executable(${projectName} ${SRC})

target_link_libraries(${projectName}
   ${Boost_LIBRARIES}
   rt
   dl
   pthread
   boost_thread
   )
