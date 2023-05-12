cmake_minimum_required(VERSION 3.22)

project(express VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

option(BUILD_TESTS "build tests" ON)
option(BUILD_EXAMPLES "build examples" ON)
option(BUILD_SSL "build with ssl support" ON)
option(FETCH_MOZ_TRUSTED_CA "fetch mozilla's trusted certificate authorities" ON)

if (BUILD_SSL)
    set(CA_BUNDLE_FILENAME ca-bundle.crt)
endif()

add_subdirectory(src)

if (BUILD_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()