add_library(file
    FileEntry.cpp
    )

target_include_directories(file
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/file"
    )

set_property(TARGET file PROPERTY POSITION_INDEPENDENT_CODE ON)
