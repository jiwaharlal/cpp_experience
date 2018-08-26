find_package( OpenCV REQUIRED )

set (CMAKE_CXX_STANDARD 11)

add_executable(searchAlgo
    searchAlgos.cpp)

target_link_libraries(searchAlgo
    PRIVATE ${OpenCV_LIBS}
    )
