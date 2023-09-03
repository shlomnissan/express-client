set(SOURCE_FILES
    "client/client.cc"
    "client/error.cc"
    "client/error.h"
    "client/timeout.h"
    "http/data_readers.h"
    "http/data_readers.cc"
    "http/defs.h"
    "http/headers.cc"
    "http/method.cc"
    "http/request_builder.cc"
    "http/request_builder.h"
    "http/response_parser.cc"
    "http/response_parser.h"
    "http/status_line.cc"
    "http/status_line.h"
    "http/validators.h"
    "net/endpoint.cc"
    "net/endpoint.h"
    "net/socket.h"
    "net/url.cc"
    "net/url.h"
    "utils/string_transformers.cc"
    "utils/string_transformers.h"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    list(APPEND SOURCE_FILES 
        "net/winsock.h"
        "net/socket_windows.cc"
    )
elseif()
    list(APPEND SOURCE_FILES
        "net/socket_unix.cc"
    )
endif()

set(PUBLIC_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/express_client_export.h"
    "${CMAKE_SOURCE_DIR}/include/express/client.h"
    "${CMAKE_SOURCE_DIR}/include/express/config.h"
    "${CMAKE_SOURCE_DIR}/include/express/exception.h"
    "${CMAKE_SOURCE_DIR}/include/express/headers.h"
    "${CMAKE_SOURCE_DIR}/include/express/method.h"
    "${CMAKE_SOURCE_DIR}/include/express/response.h"
    "${CMAKE_SOURCE_DIR}/include/express/user_auth.h"
    "${CMAKE_SOURCE_DIR}/include/express/version.h"
)

set(NAMESPACE Express)
set(EXPORT_NAME Client)
set(TARGET_NAME express_client)

add_library(${TARGET_NAME} ${SOURCE_FILES})
add_library(${NAMESPACE}::${EXPORT_NAME} ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/express>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME ${TARGET_NAME}
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

if (BUILD_SHARED_LIBS)
    set_target_properties(${TARGET_NAME} PROPERTIES
        VERSION ${express_VERSION_MAJOR}.${express_VERSION_MINOR}.${express_VERSION_PATCH}
        SOVERSION ${express_VERSION_MAJOR}
    )
endif()

configure_file(
    ${CMAKE_SOURCE_DIR}/include/express/version.h.in
    ${CMAKE_SOURCE_DIR}/include/express/version.h
    @ONLY
)

target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

include(GenerateExportHeader)
generate_export_header(${TARGET_NAME})

include(GNUInstallDirs)

install(TARGETS ${TARGET_NAME}
    EXPORT ${NAMESPACE}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/express
)

install(EXPORT ${NAMESPACE}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${NAMESPACE}
  NAMESPACE ${NAMESPACE}::
)