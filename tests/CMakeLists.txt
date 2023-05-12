cmake_minimum_required(VERSION 3.22)

find_package(GTest QUIET)
if(NOT GTEST_FOUND)
    message(STATUS "GoogleTest is not found. Consider installing it on your system.")

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG origin/main
        GIT_SHALLOW true
    )
    set(BUILD_GMOCK OFF CACHE BOOL "")
    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()
endif()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cc)

foreach(PATH IN LISTS TEST_SOURCES)
    get_filename_component(FILE_NAME ${PATH} NAME)

    string(REGEX REPLACE "\\.[^.]*$" "" NAME_NO_EXT ${FILE_NAME})

    message(STATUS "Adding test ${FILE_NAME}")

    set(TEST_TARGET run_${NAME_NO_EXT})
    add_executable(${TEST_TARGET} ${FILE_NAME})
    target_link_libraries(${TEST_TARGET} PRIVATE gtest_main express_client)
    add_test(${NAME_NO_EXT} ${TEST_TARGET})

    if(BUILD_SSL)
        find_package(OpenSSL REQUIRED)
        target_compile_definitions(${TEST_TARGET} PUBLIC BUILD_SSL)
        target_link_libraries(${TEST_TARGET} PRIVATE OpenSSL::SSL)

        # Copy ca-bundle.crt from the project directory into the target's
        # binary directory if it exists.
        if(EXISTS "${PROJECT_SOURCE_DIR}/${CA_BUNDLE_FILENAME}")
            add_custom_command(
                TARGET ${TEST_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                ${PROJECT_SOURCE_DIR}/${CA_BUNDLE_FILENAME}
                $<TARGET_FILE_DIR:${TEST_TARGET}>
            )
        endif()
    endif()
endforeach()