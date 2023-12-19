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

void bitonic_sort(int *start, int *end, unsigned long long real_bytes) {
    int *kernel_ptr;
    size_t size = (end - start);
    size_t bytes = size * sizeof(int);

    cudaDeviceProp props;
    cudaGetDeviceProperties(&props, 0);

    cudaMalloc((void **) &kernel_ptr, bytes);
    cudaMemcpy(kernel_ptr, start, bytes, cudaMemcpyHostToDevice);

    dim3 blocks(std::max(1ull, size / props.maxThreadsPerBlock), 1);
    dim3 threads(props.maxThreadsPerBlock, 1);

    int j, k;
    for (k = 2; k <= size; k <<= 1) {
        for (j = k >> 1; j > 0; j = j >> 1) {
            bitonic_sort_step<<<blocks, threads>>>(kernel_ptr, j, k);
        }
    }
    cudaMemcpy(start, kernel_ptr, real_bytes, cudaMemcpyDeviceToHost);
    cudaFree(kernel_ptr);
}

/*
void cpu_gpu_sort(int * start, int * end, unsigned int core_count) {
    using ull = unsigned long long;
    ull cpu_size = (end - start) / 5 * 2;
    ull gpu_size = (end - start) / 5 * 3 + ((end - start) % 5);
    ull cpu_bytes = cpu_size * sizeof(int);
    ull gpu_bytes = gpu_size * sizeof(int);
    int *arr1 = new int[cpu_size];
    int *arr2;

    ull padded_size = 1;
    while (padded_size < gpu_size) {
        padded_size <<= 1;
    }
    arr2 = new int[padded_size];

    memcpy(arr1, start, cpu_bytes);
    memcpy(arr2, start + cpu_size, gpu_bytes);
    for (ull i = gpu_size; i < padded_size; i++) {
        arr2[i] = INT_MAX;
    }

    auto t = std::thread(bitonic_sort, arr2, arr2 + padded_size, gpu_bytes);
    par_sort(arr1, arr1 + cpu_size, cpu_size, cpu_size / core_count, std::less<>{});
    t.join();

    std::merge(arr1, arr1 + cpu_size, arr2, arr2 + gpu_size, start);

    delete[] arr1;
    delete[] arr2;
}
*/