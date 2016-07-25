cmake_minimum_required(VERSION 2.8)

set(projectName actor)
project(${projectName})

find_package(Boost COMPONENTS program_options thread system chrono REQUIRED)

FILE (GLOB SRC *.cpp)

add_executable(${projectName} ${SRC})

target_link_libraries(${projectName}
   ${Boost_LIBRARIES}
   rt
   dl
   pthread
   boost_thread
   )
