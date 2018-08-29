set (CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED )

add_executable(a-star
    a-star.cpp)

target_link_libraries(a-star
    PRIVATE ${OpenCV_LIBS}
    )
