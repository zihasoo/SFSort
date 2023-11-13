#include <utility>
#include <algorithm>

#define THREADS 512 // 2^9
#define BLOCKS 32768 // 2^15
#define NUM_VALS THREADS*BLOCKS

template<typename iter, typename comp>
__global__ void bitonic_sort_step(iter kernel_iter, int j, int k, comp cmp) {
    unsigned int i, ixj;
    i = threadIdx.x + blockDim.x * blockIdx.x;
    ixj = i ^ j;

    if ((ixj) > i) {
        if ((i & k) == 0) {
            if (cmp(kernel_iter[ixj], kernel_iter[i])) {
                std::iter_swap(kernel_iter + i, kernel_iter + ixj);
            }
        }
        else {
            if (cmp(kernel_iter[i], kernel_iter[ixj])) {
                std::iter_swap(kernel_iter + i, kernel_iter + ixj);
            }
        }
    }
}

template<typename iter, typename comp>
void bitonic_sort(iter begin, iter end, comp cmp) {
    iter kernel_iter;
    size_t size = (end - begin) * sizeof(decltype(*begin));

    cudaMalloc(static_cast<void **>(&kernel_iter), size);
    cudaMemcpy(kernel_iter, begin, size, cudaMemcpyHostToDevice);

    dim3 blocks(BLOCKS, 1);
    dim3 threads(THREADS, 1);

    int j, k;
    for (k = 2; k <= NUM_VALS; k <<= 1) {
        for (j = k >> 1; j > 0; j = j >> 1) {
            bitonic_sort_step<<<blocks, threads>>>(kernel_iter, j, k, cmp);
        }
    }
    cudaMemcpy(begin, kernel_iter, size, cudaMemcpyDeviceToHost);
    cudaFree(kernel_iter);
}
