set(SOURCE_FILES
    "http/data.cc"
    "http/header.cc"
    "http/http_defs.cc"
    "http/request.cc"
    "http/response.cc"
    "http/transformers.cc"
    "net/endpoint.cc"
    "net/socket_secure.cc"
    "net/socket.cc"
    "net/url.cc"
    "client.cc"
    "error.cc"
)

set(NAMESPACE Express)
set(EXPORT_NAME Client)
set(TARGET_NAME express_client)

add_library(${TARGET_NAME} ${SOURCE_FILES})
add_library(${NAMESPACE}::${EXPORT_NAME} ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

set_target_properties(${TARGET_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ${BUILD_PIC}
    OUTPUT_NAME ${TARGET_NAME} 
    EXPORT_NAME ${EXPORT_NAME}
    PUBLIC_HEADER
        ${CMAKE_SOURCE_DIR}/include/express/client.h
)

target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

if (BUILD_SHARED_LIBS)
    set_target_properties(${TARGET_NAME} PROPERTIES
        VERSION ${express_VERSION_MAJOR}.${express_VERSION_MINOR}.${express_VERSION_PATCH}
        SOVERSION ${express_VERSION_MAJOR}
    )
endif()

if(BUILD_SSL)
    find_package(OpenSSL REQUIRED)
    target_compile_definitions(${TARGET_NAME} PRIVATE BUILD_SSL)
    target_link_libraries(${TARGET_NAME} PUBLIC 
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

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