cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

include(UseSWIG)
find_package(SWIG 3.0)

if(NOT SWIG_FOUND)
    message(ERROR "Could not find SWIG")
    return()
endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../file/include"
    )

set_property(SOURCE pyFile.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE pyFile.i PROPERTY SWIG_MODULE_NAME pyFile)

swig_add_library(pyFile
    TYPE SHARED
    LANGUAGE python
    SOURCES
    pyFile.i
    )

swig_link_libraries(pyFile
    PRIVATE file
    )

target_include_directories(pyFile
    PRIVATE ${PYTHON_INCLUDE_DIRS}
    PRIVATE $<TARGET_PROPERTY:file,INCLUDE_DIRECTORIES>
    "${CMAKE_CURRENT_SOURCE_DIR}../file/include"
    )

get_target_property(pyFile_DIRS pyFile INCLUDE_DIRECTORIES)
message("pyFile include dirs: ${pyFile_DIRS}")
