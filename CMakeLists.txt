cmake_minimum_required(VERSION 3.26)
project(SFSort CUDA)

set(CMAKE_CUDA_STANDARD 20)

add_subdirectory(./srcs)

set_target_properties(SFSort PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
