find_package(GTest)

if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

add_executable(testHelpers
    ../helper.cpp
    testHelpers.cpp
    )

target_include_directories(testHelpers
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

target_link_libraries(testHelpers
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    )

gtest_discover_tests(testHelpers)
