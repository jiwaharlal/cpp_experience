cmake_minimum_required ( VERSION 2.8 )

set(PROJECT curlDownloader)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )

add_subdirectory("curlCpp")

set ( SRC
   curlDownloaderMain.cpp
   )

add_executable (${PROJECT} ${SRC})

set( OpenCVLibs
    opencv_core
    opencv_imgproc
    opencv_video
    opencv_highgui
    opencv_photo
)

target_link_libraries(${PROJECT} curlCpp ${OpenCVLibs})
