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

add_library(express_client ${SOURCE_FILES} ${INCLUDE_FILES})

if(BUILD_SSL)
    # Downloads Mozilla's Trusted CAs for HTTPS certificate verification
    if (FETCH_MOZ_TRUSTED_CA)
        file(
            DOWNLOAD
            https://curl.se/ca/cacert.pem
            ${PROJECT_SOURCE_DIR}/${CA_BUNDLE_FILENAME}
            TLS_VERIFY TRUE
        )
    endif()

    # Copy ca-bundle.crt from the project directory into the target's
    # binary directory if it exists.
    if(EXISTS "${PROJECT_SOURCE_DIR}/${CA_BUNDLE_FILENAME}")
        add_custom_command(
            TARGET express_client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/${CA_BUNDLE_FILENAME}
            $<TARGET_FILE_DIR:express_client>
        )
    endif()

    find_package(OpenSSL REQUIRED)
    target_compile_definitions(express_client PUBLIC BUILD_SSL)
    target_link_libraries(express_client PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()