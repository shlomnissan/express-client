cmake_minimum_required(VERSION 3.22)

function(express_client_example example_name)
    set(EXAMPLE_EXECUTABLE run_${example_name})
    add_executable(${EXAMPLE_EXECUTABLE} ${test_name}.cc)
    target_link_libraries(${EXAMPLE_EXECUTABLE} express_client_static)
endfunction()

express_client_test(hello_client_example)