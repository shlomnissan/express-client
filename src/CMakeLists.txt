cmake_minimum_required(VERSION 3.22)

file (GLOB HTTP_SOURCE_FILES "http/*.cc")
file (GLOB NET_SOURCE_FILES "net/*.cc")
file (GLOB CLIENT_SOURCE_FILES "*.cc")

set(SOURCE_FILES
    ${HTTP_SOURCE_FILES}
    ${NET_SOURCE_FILES}
    ${CLIENT_SOURCE_FILES}
)

add_library(express_client ${SOURCE_FILES})

target_include_directories(express_client PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(express_client PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -fPIC>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

if(BUILD_SSL)
    find_package(OpenSSL REQUIRED)
    target_compile_definitions(express_client PUBLIC BUILD_SSL)
    target_link_libraries(express_client PUBLIC 
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()