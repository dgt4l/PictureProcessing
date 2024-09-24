cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

find_package(Threads REQUIRED)

set(LIB_MTCI_SOURCES
    mtci_image.h
    mtci_image.cc
)

add_library(mtci ${LIB_MTCI_SOURCES})
target_include_directories(mtci PUBLIC ${CMAKE_CURRENT_BINARY_DIR})