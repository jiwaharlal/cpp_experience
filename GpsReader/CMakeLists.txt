cmake_minimum_required(VERSION 2.8)

set(PROJECT devReader)

find_package(Boost COMPONENTS program_options REQUIRED)

set(LIBS ${Boost_LIBRARIES})

file(GLOB_RECURSE SRC app/*.cpp)

# minmea
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SRC ${SRC} minmea/minmea.c)

# remove entry point file
foreach(SRC_FILE int ${SRC})
   string(FIND ${SRC_FILE} DevReader.cpp POS)
   if(NOT ${POS} EQUAL -1)
      list(REMOVE_ITEM SRC ${SRC_FILE})
      break()
   endif()
endforeach()

#list(REMOVE_ITEM SRC DevReader.cpp)
message("Src without main: ${SRC}")

# tests
file(GLOB_RECURSE TEST_SRC test/*.cpp)
set(TEST test_${PROJECT})
message("${TEST} ${TEST_SRC}")
include_directories(app)
add_executable(${TEST} ${SRC} ${TEST_SRC})
set_property(TARGET ${TEST} PROPERTY C_STANDARD 99)
target_link_libraries(${TEST}
   ${LIBS})
enable_testing()
add_test(${TEST} ${TEST})

message("Source: ${SRC}")

set(EXECUTABLE_NAME ${PROJECT})

add_executable(${EXECUTABLE_NAME} ${SRC} app/DevReader.cpp)
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY C_STANDARD 99)

target_link_libraries(${EXECUTABLE_NAME} 
   ${LIBS})
