cmake_minimum_required (VERSION 2.8)

set (EXECUTABLE_NAME boostPractice)

add_definitions(-std=c++03)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )

find_package(Boost COMPONENTS filesystem program_options thread system chrono coroutine context REQUIRED)

#FILE (GLOB SRC src/*.cpp)
set (SRC src/practiceMain.cpp)

add_executable (${EXECUTABLE_NAME} ${SRC})

message ("All boost libs: ${Boost_LIBRARIES}")

#set (REQUIRED_LIBS_KEYWORDS chrono system filesystem)
#set (Boost_REDUCED_LIB_LIST)
#foreach (BOOST_LIB ${Boost_LIBRARIES})
   #foreach(KEYWORD ${REQUIRED_LIBS_KEYWORDS})
      #string (FIND ${BOOST_LIB} ${KEYWORD} KEYWORD_POS)
      #if (${KEYWORD_POS} GREATER 0)
         #list (APPEND Boost_REDUCED_LIB_LIST ${BOOST_LIB})
      #endif()
   #endforeach()
#endforeach()

#message ("Reduced list: ${Boost_REDUCED_LIB_LIST}")

target_link_libraries(${EXECUTABLE_NAME}
   #${LIBRARY_NAME}
   ${Boost_LIBRARIES}
   rt
   dl
   pthread
   boost_thread
   )
