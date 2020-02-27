cmake_minimum_required(VERSION 3.15)

project(cmake_sandbox)
add_definitions(-DUSE_GUI=1)
#set(USE_GUI 0)

include(cmake/conan-recipe.cmake)
add_subdirectory(Lib.A)
add_subdirectory(Lib.2)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt VERSION_STRING)
message(STATUS "string from file: ${VERSION_STRING}")
string(REGEX MATCH "(\\d+)" VERSION_COMPONENTS ${VERSION_STRING})
#string(REGEX MATCHALL "(+)\.(\d+)\.(\d+)" VERSION_COMPONENTS ${VERSION_STRING})
message(STATUS "parsed version components: ${VERSION_COMPONENTS}")
string(FIND ${VERSION_STRING} "." DOT_POS)
message(STATUS "dot found at ${DOT_POS}")

string(REPLACE "." ";" VERSION_COMPONENTS ${VERSION_STRING})
list(GET VERSION_COMPONENTS 0 MIMICS_VERSION_MAJOR)
list(GET VERSION_COMPONENTS 1 MIMICS_VERSION_MINOR)
list(GET VERSION_COMPONENTS 2 MIMICS_VERSION_PATCH)
message(STATUS "MIMICS_VERSION_MAJOR : ${MIMICS_VERSION_MAJOR}")

file(GLOB SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
message(STATUS "Source files: ${SOURCE_FILES}")

if (NOT USE_GUI)
    message(STATUS "Build without GUI")
endif()

message("USE_GUI : ${USE_GUI}")

add_executable(app
    app.cpp
   )
