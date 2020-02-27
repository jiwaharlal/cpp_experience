add_conan_recipe(Lib.1)

add_library(Lib.1
    add.cpp)

target_include_directories(Lib.1
    PUBLIC include
    )
