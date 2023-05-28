cmake_minimum_required(VERSION 3.22)

file(READ "version.txt" VERSION_FILE)
project(express VERSION ${VERSION_FILE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "build tests" ON)
option(BUILD_EXAMPLES "build examples" ON)
option(BUILD_SSL "build with ssl support" ON)
option(BUILD_PIC "build position independent code" ON)
option(FETCH_MOZ_TRUSTED_CA "fetch mozilla's trusted certificate authorities" ON)

add_subdirectory(src)

if (BUILD_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

set(VERSION_MAJOR ${express_VERSION_MAJOR})
set(VERSION_MINOR ${express_VERSION_MINOR})
set(VERSION_PATCH ${express_VERSION_PATCH})
set(VERSION_GIT_DATE ${express_VERSION_TWEAK})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/express/version.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/include/express/version.h
    @ONLY
)