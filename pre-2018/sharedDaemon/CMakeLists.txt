cmake_minimum_required(VERSION 2.8)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

option(BUILD_TEST_LIB "Build test library which will not really calculate." ON)

set(EXECUTABLE_NAME daemonLoader) 

find_package(Boost COMPONENTS program_options thread system chrono REQUIRED)

set(LIB_REAL_VERSION 0)
set(LIB_TEST_VERSION 42)
add_definitions(-DCALCULATOR_REAL_VERSION="${LIB_REAL_VERSION}")
add_definitions(-DCALCULATOR_TEST_VERSION="${LIB_TEST_VERSION}")

if (${BUILD_TEST_LIB})
   add_subdirectory("calculatorTest")
else()
   add_subdirectory("calculator")
endif()

remove_definitions(-DCALCULATOR_TEST_VERSION)
remove_definitions(-DCALCULATOR_REAL_VERSION)

file(GLOB APP_SRC app/*.cpp)
add_executable(${EXECUTABLE_NAME} ${APP_SRC})

target_link_libraries(${EXECUTABLE_NAME} 
   #${LIBRARY_NAME}
   ${Boost_LIBRARIES}
   rt
   dl
   )
