cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

add_executable(rrt-vehicle
    rrt-vehicle.cpp)

target_link_libraries(rrt-vehicle
    PRIVATE ${OpenCV_LIBS}
    PRIVATE Boost::boost
    )
