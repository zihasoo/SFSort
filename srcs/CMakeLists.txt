
add_executable(
        SFSort
        bitonic_sort.cu
        main.cu
        sf_sort.hpp
        data_generator.hpp
        thread_pool.cu
        thread_pool.h
        utility.cu
        utility.h
        bitonic_sort.h
)