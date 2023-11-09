cmake_minimum_required(VERSION 3.26)
project(SFSort CUDA)

set(CMAKE_CUDA_STANDARD 20)

add_executable(SFSort main.cu
        bitonic_sort.cu)

set_target_properties(SFSort PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
