cmake_minimum_required(VERSION 2.8)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )

set(EXECUTABLE_NAME daemonLoader) 
set(LIBRARY_NAME calculator)

find_package(Boost COMPONENTS program_options thread system chrono REQUIRED)

file(GLOB_RECURSE LIB_SRC calculator/*.cpp)

add_library(${LIBRARY_NAME} SHARED ${LIB_SRC})

add_executable(${EXECUTABLE_NAME} app/daemonLoader.cpp)

link_directories(Boost_LIBRARY_DIRS)

target_link_libraries(${EXECUTABLE_NAME} 
   ${LIBRARY_NAME}
   ${Boost_LIBRARIES}
   rt
   )
