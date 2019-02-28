find_package(GTest)
if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

add_executable(testRing
    testRing.cpp)

target_link_libraries(testRing
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    )

add_executable(testRing2
    testRing.cpp)

target_link_libraries(testRing2
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
    )

#enable_testing()
gtest_add_tests(testRing "" AUTO)

set(TEST_RING testRing)

#set(unit_tests "${TEST_RING} && ${TEST_RING}")

add_custom_target(test1 ${TEST_RING})
add_custom_target(test2 COMMAND testRing2 DEPENDS test1)

set(TESTS testRing testRing2)

list(LENGTH TESTS test_count)
math(EXPR range_limit "${test_count} - 1")
foreach(i RANGE ${range_limit})
    list(GET TESTS ${i} elt)
    message("${i}-th element: ${elt}")
endforeach()

foreach(t ${TESTS})
    set(testTarget "test_target_${t}")
    message("Target: ${testTarget}")
endforeach()

#gtest_discover_tests(testRing)

#add_test(NAME RingTest COMMAND testRing)
#add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  #DEPENDS testRing)

