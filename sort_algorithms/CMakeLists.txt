
project(SortAlgorithms)

add_library(
        SortAlgorithms
        insertion_sort.cpp
        intro_sort.cpp
        intro_sort_utility.cpp
        parallel_intro_sort.cpp
        thread_pool_parallel_intro_sort.cpp
)

target_include_directories(SortAlgorithms PUBLIC ${CMAKE_SOURCE_DIR}/includes)