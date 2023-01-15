cmake_minimum_required(VERSION 3.25)

function(simple_http_test test_name)
    set(TEST_EXECUTABLE run_${test_name})
    add_executable(${TEST_EXECUTABLE} ${test_name}.cpp)
    target_link_libraries(${TEST_EXECUTABLE} GTest::gtest GTest::gtest_main simple_http_static)
    add_test(${test_name} ${TEST_EXECUTABLE})
endfunction()

simple_http_test(client_test)
simple_http_test(endpoint_test)
simple_http_test(socket_test)