cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)
add_compile_options(-g)

include_directories(lib root)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(utils)
add_subdirectory(mtci)
add_subdirectory(root)
add_subdirectory(client)