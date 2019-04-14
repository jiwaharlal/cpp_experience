find_package(GTest)
if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

#SET(GCC_COVERAGE_COMPILE_FLAGS "-g -fprofile-arcs -ftest-coverage --coverage")
#SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(testMaxHeap
    testMaxHeap.cpp
    )

target_link_libraries(testMaxHeap
    PRIVATE maxHeap
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    PRIVATE gcov
    )

add_executable(testSegmentTree
    testSegmentTree.cpp
    )

target_link_libraries(testSegmentTree
    PRIVATE maxHeap
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    PRIVATE gcov
    )

gtest_discover_tests(testMaxHeap)
gtest_discover_tests(testSegmentTree)
