cmake_minimum_required(VERSION 2.8)

set(projectName propertyTree)
project(${projectName})

set(CMAKE_AUTOMOC ON)

FIND_PACKAGE ( Qt5Widgets REQUIRED )

FILE (GLOB SRC src/*.cpp)

message("Sources: ${SRC}")

add_executable(${projectName} ${SRC})

QT5_USE_MODULES ( ${projectName} Core Gui Widgets )
