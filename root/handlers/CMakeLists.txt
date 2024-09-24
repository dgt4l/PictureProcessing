cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

find_package(Threads REQUIRED)

find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_library(
    ZeroMQ_LIBRARY
    NAMES zmq
    PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
)


set(HELPERS_SOURCES
    command_dispatcher.h
    command_dispatcher.cc
    response_handler.h
    response_handler.cc
)

add_library(helper ${HELPERS_SOURCES})

target_link_libraries(helper PRIVATE ${ZeroMQ_LIBRARY})
target_link_libraries(helper PRIVATE utils)
target_link_libraries(helper PRIVATE manager)

target_include_directories(helper PRIVATE ${CMAKE_CURRENT_BINARY_DIR})