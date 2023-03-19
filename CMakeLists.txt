cmake_minimum_required(VERSION 3.22)

project(express VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

option(BUILD_TESTS "build tests" ON)
option(BUILD_SHARED_LIBS "build shared library" ON)
option(BUILD_EXAMPLES "build examples" ON)

include_directories(include)
add_subdirectory(src)

if (BUILD_TESTS)
    include(CTest)
    find_package(GTest QUIET)
    if (NOT GTEST_FOUND)
        message("GoogleTest is not found. Consider installing it on your system.")
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
    add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()