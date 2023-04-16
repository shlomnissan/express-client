cmake_minimum_required(VERSION 3.22)

file (GLOB HTTP_SOURCE_FILES "http/*.cc")
file (GLOB NET_SOURCE_FILES "net/*.cc")
file (GLOB CLIENT_SOURCE_FILES "*.cc")

set(SOURCE_FILES
    ${HTTP_SOURCE_FILES}
    ${NET_SOURCE_FILES}
    ${CLIENT_SOURCE_FILES}
)

file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(express_client OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

if(BUILD_SSL)
    find_package(OpenSSL REQUIRED)
    target_compile_definitions(express_client PUBLIC BUILD_SSL)
    target_link_libraries(express_client PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()