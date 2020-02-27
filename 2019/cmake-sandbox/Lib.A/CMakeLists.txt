add_conan_recipe(Lib.A)

add_library(Lib.A
    add.cpp)

target_include_directories(Lib.A
    PUBLIC include
    )
