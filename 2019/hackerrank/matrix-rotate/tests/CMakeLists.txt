find_package(GTest)
if (NOT GTest_FOUND)
    message(warning "GTest not found, tests will not be built")
    return()
endif()

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage --coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(testRing
    ../src/Ring.cpp
    testRing.cpp
    )

target_link_libraries(testRing
    #PRIVATE ring
    PRIVATE GTest::GTest
    #PRIVATE GTest::Main
    )

#add_executable(testRing2
    #testRing.cpp)

#target_link_libraries(testRing2
    #PRIVATE GTest::GTest
    #PRIVATE GTest::Main
    #)

#gtest_add_tests(testRing "" AUTO)

gtest_discover_tests(testRing)

#add_test(NAME RingTest COMMAND testRing)
#add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  #DEPENDS testRing)

#set(TEST_RING testRing)

#set(TEST_EXECUTABLES testRing testRing2)

#foreach(t ${TEST_EXECUTABLES})
    #set(test_target "run_${t}")
    #add_custom_target(${test_target} COMMAND ${t})
    #list(APPEND TEST_TARGETS ${test_target})
#endforeach()

#add_custom_target(test DEPENDS ${TEST_TARGETS})
