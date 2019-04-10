cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(OpenCV 3.2 REQUIRED PATHS /usr/local)
find_package(Boost REQUIRED)

add_executable(rrt-vehicle
    rrt-vehicle.cpp)

message("CV libs: ${OpenCV_LIBS}")
message("CV include dirs: ${OpenCV_INCLUDE_DIRS}")

target_link_libraries(rrt-vehicle
    PRIVATE Boost::boost
    PRIVATE ${OpenCV_LIBS}
    )
