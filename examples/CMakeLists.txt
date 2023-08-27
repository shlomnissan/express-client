message(STATUS "🛠 Building examples")
list(APPEND CMAKE_MESSAGE_INDENT "   -- ")

add_executable(hello_client hello_client.cc)

target_link_libraries(hello_client Express::Client)