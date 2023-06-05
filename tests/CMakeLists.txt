find_package(GTest QUIET)

set(BUILD_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")

if(NOT GTEST_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG origin/main
        GIT_SHALLOW true
    )
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()
endif()

if (BUILD_SSL AND FETCH_MOZ_TRUSTED_CA)
    # Downloads Mozilla's Trusted CAs for certificate verification
    file(
        DOWNLOAD
        https://curl.se/ca/cacert.pem
        ${CMAKE_CURRENT_BINARY_DIR}/ca-bundle.crt
        TLS_VERIFY TRUE
    )
endif()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cc)
foreach(PATH IN LISTS TEST_SOURCES)
    get_filename_component(FILE_NAME ${PATH} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" NAME_NO_EXT ${FILE_NAME})
    message(STATUS "🧪 Adding test ${FILE_NAME}")

    set(TEST_TARGET run_${NAME_NO_EXT})
    
    add_executable(${TEST_TARGET} ${FILE_NAME})
    target_link_libraries(${TEST_TARGET} PRIVATE gtest_main Express::Client)
    add_test(${NAME_NO_EXT} ${TEST_TARGET})
endforeach()