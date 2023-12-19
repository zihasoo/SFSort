cmake_minimum_required(VERSION 3.26)

project(Main)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./utility)
add_subdirectory(./sort_algorithms)
add_executable(Main main.cpp)

target_link_libraries(Main PUBLIC Utility)
target_link_libraries(Main PUBLIC SortAlgorithms)
