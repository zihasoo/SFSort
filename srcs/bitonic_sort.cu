#include "bitonic_sort.h"

__global__ void bitonic_sort_step(int *ptr, int j, int k) {
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
        } else {
            if (ptr[ixj] > ptr[i]) {
                temp = ptr[i];
                ptr[i] = ptr[ixj];
                ptr[ixj] = temp;
            }
        }
    }
}

void checkCuda(cudaError_t err) {
    if (err != cudaSuccess) {
        std::cout << cudaGetErrorString(err) << '\n';
    }
}

void bitonic_sort(int *begin, int *end, unsigned long long real_bytes) {
    int *kernel_ptr;
    size_t size = (end - begin);
    size_t bytes = size * sizeof(int);

    cudaDeviceProp props;
    cudaGetDeviceProperties(&props, 0);

    checkCuda(cudaMalloc((void **) &kernel_ptr, bytes));
    cudaMemcpy(kernel_ptr, begin, bytes, cudaMemcpyHostToDevice);

    dim3 blocks(std::max(1ull, size / props.maxThreadsPerBlock), 1);
    dim3 threads(props.maxThreadsPerBlock, 1);

    int j, k;
    for (k = 2; k <= size; k <<= 1) {
        for (j = k >> 1; j > 0; j = j >> 1) {
            bitonic_sort_step<<<blocks, threads>>>(kernel_ptr, j, k);
        }
    }
    checkCuda(cudaMemcpy(begin, kernel_ptr, real_bytes, cudaMemcpyDeviceToHost));
    checkCuda(cudaFree(kernel_ptr));
}
