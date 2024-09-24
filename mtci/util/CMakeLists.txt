cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

find_package(Threads REQUIRED)

set(UTIL_MTCI_SOURCES
    mtci_threads.h
    mtci_threads.cc
    mtci_transform.h
    mtci_transform.cc
)

add_library(mtci_util ${UTIL_MTCI_SOURCES})

target_link_libraries(mtci_util PUBLIC mtci)
target_include_directories(mtci_util PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
