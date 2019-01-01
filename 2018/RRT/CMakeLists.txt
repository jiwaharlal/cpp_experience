cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED )

add_executable(rrt
    rrt.cpp)

target_link_libraries(rrt
    PRIVATE ${OpenCV_LIBS}
    )
