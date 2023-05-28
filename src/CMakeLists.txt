file (GLOB HTTP_SOURCE_FILES "http/*.cc")
file (GLOB NET_SOURCE_FILES "net/*.cc")
file (GLOB CLIENT_SOURCE_FILES "*.cc")

set(SOURCE_FILES
    ${HTTP_SOURCE_FILES}
    ${NET_SOURCE_FILES}
    ${CLIENT_SOURCE_FILES}
)

set(TARGET express_client)
add_library(${TARGET} ${SOURCE_FILES})
target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_PIC})
target_compile_options(${TARGET} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

if (BUILD_SHARED_LIBS)
    set_target_properties(${TARGET} PROPERTIES
        VERSION ${express_VERSION_MAJOR}.${express_VERSION_MINOR}.${express_VERSION_PATCH}
        SOVERSION ${express_VERSION_MAJOR}
    )
endif()

if(BUILD_SSL)
    find_package(OpenSSL REQUIRED)
    target_compile_definitions(${TARGET} PRIVATE BUILD_SSL)
    target_link_libraries(${TARGET} PUBLIC 
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()