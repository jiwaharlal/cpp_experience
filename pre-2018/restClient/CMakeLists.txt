cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system context coroutine log REQUIRED)

add_executable(restClient
    rest-client.cpp
    )

target_link_libraries(
    restClient
    PRIVATE restc-cpp
    PRIVATE restclient-cpp
    PRIVATE pthread
    PRIVATE Boost::context
    PRIVATE Boost::coroutine
    PRIVATE Boost::system
    PRIVATE Boost::log
    PRIVATE ssl
    PRIVATE crypto
    PRIVATE z
    )
