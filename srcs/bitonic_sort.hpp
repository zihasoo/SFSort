#include <utility>
#include <algorithm>

#define THREADS 512 // 2^9
#define BLOCKS 32768 // 2^15
#define NUM_VALS THREADS*BLOCKS

template<typename iter, typename comp>
__global__ void bitonic_sort_step(iter ptr, int j, int k, comp cmp) {
    unsigned int i, ixj;
    i = threadIdx.x + blockDim.x * blockIdx.x;
    ixj = i ^ j;

    if ((ixj) > i) {
        if ((i & k) == 0) {
            if (cmp(ptr[ixj], ptr[i])) {
                std::iter_swap(ptr + i, ptr + ixj);
            }
        }
        else {
            if (cmp(ptr[i], ptr[ixj])) {
                std::iter_swap(ptr + i, ptr + ixj);
            }
        }
    }
}

template<typename iter, typename comp>
void bitonic_sort(iter begin, iter end, comp cmp) {
    iter kernel_ptr;
    size_t size = (end - begin) * sizeof(decltype(*begin));

    cudaMalloc(static_cast<void **>(&kernel_ptr), size);
    cudaMemcpy(kernel_ptr, begin, size, cudaMemcpyHostToDevice);

    dim3 blocks(BLOCKS, 1);
    dim3 threads(THREADS, 1);

    int j, k;
    for (k = 2; k <= NUM_VALS; k <<= 1) {
        for (j = k >> 1; j > 0; j = j >> 1) {
            bitonic_sort_step<<<blocks, threads>>>(kernel_ptr, j, k, cmp);
        }
    }
    cudaMemcpy(begin, kernel_ptr, size, cudaMemcpyDeviceToHost);
    cudaFree(kernel_ptr);
}