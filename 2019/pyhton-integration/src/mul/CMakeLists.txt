add_library(mul
    mul.cpp
    )

target_include_directories(mul
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mul"
    )

target_link_libraries(mul
    PUBLIC file
    )
