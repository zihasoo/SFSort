#include <utility>
#include "bitonic_sort.h"

__global__ void bitonic_sort_step(int* ptr, int j, int k) {
    unsigned int i, ixj;
    i = threadIdx.x + blockDim.x * blockIdx.x;
    ixj = i ^ j;
    int temp;

    if ((ixj) > i) {
        if ((i & k) == 0) {
            if (ptr[ixj] < ptr[i]) {
                temp = ptr[i];
                ptr[i] = ptr[ixj];
                ptr[ixj] = temp;
            }
        }
        else {
            if (ptr[ixj] > ptr[i]) {
                temp = ptr[i];
                ptr[i] = ptr[ixj];
                ptr[ixj] = temp;
            }
        }
    }
}

void bitonic_sort(int* begin, int* end) {
    int* kernel_ptr;
    size_t size = (end - begin) * sizeof(int);

    cudaMalloc((void **)(&kernel_ptr), size);
    cudaMemcpy(kernel_ptr, begin, size, cudaMemcpyHostToDevice);

    dim3 blocks(BLOCKS, 1);
    dim3 threads(THREADS, 1);

    int j, k;
    for (k = 2; k <= NUM_VALS; k <<= 1) {
        for (j = k >> 1; j > 0; j = j >> 1) {
            bitonic_sort_step<<<blocks, threads>>>(kernel_ptr, j, k);
        }
    }
    cudaMemcpy(begin, kernel_ptr, size, cudaMemcpyDeviceToHost);
    cudaFree(kernel_ptr);
}
