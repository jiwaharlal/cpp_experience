find_package(GTest)
if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

#add_subdirectory(gtest)

add_executable(testRing
    testRing.cpp)

target_link_libraries(testRing
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    )

enable_testing()
gtest_add_tests(
    testRing
    "" AUTO
    )

gtest_discover_tests(testRing)

add_test(NAME RingTest COMMAND testRing)
#add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  #DEPENDS testRing)
