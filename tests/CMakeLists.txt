cmake_minimum_required(VERSION 3.22)

function(express_client_test test_name)
    set(TEST_EXECUTABLE run_${test_name})
    add_executable(${TEST_EXECUTABLE} ${test_name}.cc)
    target_link_libraries(${TEST_EXECUTABLE} GTest::gtest GTest::gtest_main express_client_static)
    add_test(${test_name} ${TEST_EXECUTABLE})

    if (WIN32)
        add_custom_command(
            TARGET ${TEST_EXECUTABLE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${TEST_EXECUTABLE}> $<TARGET_FILE_DIR:${TEST_EXECUTABLE}>
            COMMAND_EXPAND_LISTS
        )
    endif ()
endfunction()

express_client_test(data_test)
express_client_test(client_test)
express_client_test(endpoint_test)
express_client_test(header_test)
express_client_test(request_test)
express_client_test(response_test)
express_client_test(socket_test)
express_client_test(url_test)
express_client_test(transformers_test)