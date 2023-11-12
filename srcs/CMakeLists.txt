
add_executable(
        SFSort
        bitonic_sort.cu
        main.cu
        sf_sort.hpp
        data_generator.hpp
        thread_pool.cpp
        thread_pool.h
)