cmake_minimum_required(VERSION 3.5)

project(project_mtci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

find_package(Threads REQUIRED)

include_directories(lib util)

add_subdirectory(lib)
add_subdirectory(util)

set(MTCI_MAIN_SOURCES
    main.cpp
)

add_executable(mtci_main ${MTCI_MAIN_SOURCES})

target_link_libraries(mtci_main PRIVATE mtci)
target_link_libraries(mtci_main PRIVATE mtci_util)