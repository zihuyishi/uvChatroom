cmake_minimum_required(VERSION 3.1)
project(uvChatRoom)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libuv/include")

add_subdirectory(src)


#client 
set(CLIENT_SOURCE src/client/client.cpp)

add_executable(client ${CLIENT_SOURCE})
target_link_libraries(client chatCore)
#server
set(SERVER_SOURCE src/server/server.cpp)

add_executable(server ${SERVER_SOURCE})
target_link_libraries(server chatCore)

#add test

