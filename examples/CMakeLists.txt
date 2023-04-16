cmake_minimum_required(VERSION 3.22)

function(express_client_example example_name)
    set(EXAMPLE_TARGET run_${example_name})
    add_executable(${EXAMPLE_TARGET} ${example_name}.cc)
    target_link_libraries(${EXAMPLE_TARGET} express_client)

    if(BUILD_SSL)
        target_compile_definitions(${EXAMPLE_TARGET} PUBLIC BUILD_SSL)
    endif()
endfunction()

express_client_example(hello_client_example)