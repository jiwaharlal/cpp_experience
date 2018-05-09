cmake_minimum_required ( VERSION 2.8 )

set ( SRC CurlRequest.cpp )

add_library ( curlCpp STATIC ${SRC} )

target_link_libraries ( curlCpp curl )
