cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED )

add_executable(d-star
    src/d-star.cpp)

target_link_libraries(d-star
    PRIVATE ${OpenCV_LIBS}
    )
