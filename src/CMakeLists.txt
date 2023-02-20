cmake_minimum_required(VERSION 3.22)

file (GLOB HTTP_SOURCE_FILES "http/*.cc")
file (GLOB NET_SOURCE_FILES "net/*.cc")
file (GLOB CLIENT_SOURCE_FILES "*.cc")

set(SOURCE_FILES
    ${HTTP_SOURCE_FILES}
    ${NET_SOURCE_FILES}
    ${CLIENT_SOURCE_FILES}
)

message(STATUS, ${SOURCE_FILES})
file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(express OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})
target_compile_features(express PRIVATE cxx_std_20)

set(express_client_INCLUDE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_include_directories(express PUBLIC ${express_client_INCLUDE})

if (BUILD_SHARED_LIBS)
    add_library(express_client_shared SHARED $<TARGET_OBJECTS:express>)
    target_include_directories(express_client_shared INTERFACE ${express_client_INCLUDE})
    set_target_properties(express_client_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
endif()

add_library(express_client_static STATIC $<TARGET_OBJECTS:express>)
target_include_directories(express_client_static INTERFACE ${express_client_INCLUDE})
set_target_properties(express_client_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})