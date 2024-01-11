cmake_minimum_required(VERSION 3.5)

project(mtci)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )

enable_testing()


add_library(mtci_lib.a lib/mtci_image.cc)
set_target_properties(mtci_lib.a PROPERTIES PREFIX "" SUFFIX "")

# target_link_libraries(
#  gtest_main
#  gtest
#)

add_executable(mtci main.cpp)

target_link_libraries(mtci "mtci_lib.a")

add_test(build "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target mtci)
add_test(leak valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./mtci)
set_tests_properties(leak PROPERTIES DEPENDS build)