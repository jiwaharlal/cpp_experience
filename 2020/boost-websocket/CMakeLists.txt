cmake_minimum_required(VERSION 3.0)
project(BoostWebSockets)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

find_package(Boost 1.72 COMPONENTS)
find_package(OpenSSL)

include_directories(common)

add_subdirectory(client-sync)
add_subdirectory(client-sync-ssl)
add_subdirectory(server-sync)
