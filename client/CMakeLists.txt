cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)
add_compile_options(-g)

find_package(Threads REQUIRED)

include_directories(..)

include_directories(handlers)

set(CLIENT_SOURCES
    worker.cc
)

add_subdirectory(handlers)

add_executable(client ${CLIENT_SOURCES})

target_link_libraries(client PRIVATE helper2)