message(STATUS "🧪 Building tests")
list(APPEND CMAKE_MESSAGE_INDENT "   -- ")

set(BUILD_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")

find_package(GTest QUIET)

if (NOT GTEST_FOUND)
    message(STATUS "GoogleTest was not found, fetching it from GitHub.")
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

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/**/*.cc)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

foreach(TEST IN LISTS TEST_SOURCES)
    get_filename_component(FILE_NAME ${TEST} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" NAME_NO_EXT ${FILE_NAME})
    message(STATUS "Adding test ${FILE_NAME}")

    set(TEST_TARGET run_${NAME_NO_EXT})
    add_executable(${TEST_TARGET} ${TEST})
    target_link_libraries(${TEST_TARGET} PRIVATE gtest_main Express::Client)
    add_test(${NAME_NO_EXT} ${TEST_TARGET})
endforeach()

if (CODE_COVERAGE)
    setup_target_for_coverage_gcovr_xml(
        NAME ctest_coverage 
        EXECUTABLE ctest
        EXCLUDE
            "${PROJECT_SOURCE_DIR}/examples/"
            "${PROJECT_SOURCE_DIR}/build/_deps/"
            "/usr/"
    )
endif()