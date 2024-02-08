cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

include_directories(../lib)

find_package(Threads REQUIRED)

find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ QUIET zmq)

include_directories(..)

find_library(
    ZeroMQ_LIBRARY
    NAMES zmq
    PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
)

set(UTILS_SOURCES
    auto_handler.cc
    auto_handler.h
)

add_library(utils ${UTILS_SOURCES})

target_link_libraries(utils PRIVATE ${ZeroMQ_LIBRARY})
target_include_directories(utils PRIVATE ${CMAKE_CURRENT_BINARY_DIR})