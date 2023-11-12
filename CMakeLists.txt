cmake_minimum_required(VERSION 3.26)
project(SFSort CUDA)

set(CMAKE_CUDA_STANDARD 20)

add_executable(SFSort
        bitonic_sort.cu
        main.cu
        sf_sort.hpp
        data_generator.hpp
)

add_compile_options(-O2)

set_target_properties(SFSort PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
