cmake_minimum_required ( VERSION 2.8 )

find_package(Boost COMPONENTS system thread REQUIRED)

set ( SRC CurlRequest.cpp )

add_library ( curlCpp SHARED ${SRC} )

set_target_properties(curlCpp
    PROPERTIES
        VERSION 1.0
        SOVERSION 1
    )

target_link_libraries ( curlCpp 
    curl
    pthread
    Boost::system
    Boost::thread
    )
