cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

find_package(Threads REQUIRED)

include_directories(..)

# add_subdirectory(../zmq ${CMAKE_CURRENT_BINARY_DIR}/zmq)

include_directories(handlers)

set(SERVER_SOURCES
    server.cc
)

add_subdirectory(handlers)
add_subdirectory(manager)

add_executable(server ${SERVER_SOURCES})

target_link_libraries(server PRIVATE manager)
target_link_libraries(server PRIVATE helper)