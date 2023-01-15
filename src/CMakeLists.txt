cmake_minimum_required(VERSION 3.25)

file(GLOB SOURCE_FILES "*.cpp")
file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(simple_http OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})
target_compile_features(simple_http PRIVATE cxx_std_20)

set(SIMPLE_HTTP_INCLUDE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_include_directories(simple_http PUBLIC ${SIMPLE_HTTP_INCLUDE})

if (BUILD_SHARED_LIBS)
    add_library(simple_http_shared SHARED $<TARGET_OBJECTS:simple_http>)
    target_include_directories(simple_http_shared INTERFACE ${SIMPLE_HTTP_INCLUDE})
    set_target_properties(simple_http_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
endif()

add_library(simple_http_static STATIC $<TARGET_OBJECTS:simple_http>)
target_include_directories(simple_http_static INTERFACE ${SIMPLE_HTTP_INCLUDE})
set_target_properties(simple_http_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})