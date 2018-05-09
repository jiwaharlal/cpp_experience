cmake_minimum_required( VERSION 2.8 )

set( ProjectName Heaps )
project( ${ProjectName} )

set( SRC
    src/main.cpp
)

add_executable( ${ProjectName} ${SRC} )
