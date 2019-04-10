cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

add_executable(rrt
    rrt.cpp)

target_link_libraries(rrt
    PRIVATE ${OpenCV_LIBS}
    PRIVATE Boost::boost
    )
