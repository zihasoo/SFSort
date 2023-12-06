#ifndef SF_SORT_HPP
#define SF_SORT_HPP

#include <thread>
#include <future>
#include <utility>
#include <execution>
#include <algorithm>

#include "bitonic_sort.h"
#include "thread_pool.h"

template<typename iter, typename comp>
void insertion_sort(iter start, iter end, comp cmp) {
    iter i, j;
    if (start == end) return;
    for (i = start + 1; i != end; ++i) {
        auto key(std::move(*i));
        for (j = i - 1; j >= start && cmp(key, *j); --j) {
            *(j + 1) = std::move(*j);
        }
        *(j + 1) = std::move(key);
    }
}

template<typename iter, typename comp>
void set_pivot(iter dest, iter a, iter b, iter c, comp cmp) {
    if (cmp(*a, *b)) {
        if (cmp(*b, *c))
            std::iter_swap(dest, b);
        else if (cmp(*a, *c))
            std::iter_swap(dest, c);
        else
            std::iter_swap(dest, a);
    } else if (cmp(*a, *c))
        std::iter_swap(dest, a);
    else if (cmp(*b, *c))
        std::iter_swap(dest, c);
    else
        std::iter_swap(dest, b);
}

template<typename iter, typename comp>
iter partition(iter pivot, iter start, iter end, comp cmp) {
    while (true) {
        do start++;
        while (cmp(*start, *pivot));
        do end--;
        while (cmp(*pivot, *end));
        if (end <= start) {
            std::iter_swap(end, pivot);
            return end;
        }
        std::iter_swap(end, start);
    }
}

template<typename iter, typename comp>
void intro_sort(iter start, iter end, unsigned int depth_check, comp cmp) {
    if (start + 1 >= end) return;

    if (end - start <= 32) {
        insertion_sort(start, end, cmp);
        return;
    }

    if (depth_check <= 0) {
        std::make_heap(start, end, cmp);
        std::sort_heap(start, end, cmp);
        return;
    }

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1, cmp);
    iter pivot = partition(start, start, end, cmp);

    depth_check = (depth_check >> 1) + (depth_check >> 2);
    intro_sort(start, pivot, depth_check, cmp);
    intro_sort(pivot + 1, end, depth_check, cmp);
}

//스레드 풀을 루트에서 만들어서 넘겨줘야 함.
template<typename iter, typename comp>
void par_pool_sort(iter start, iter end, unsigned int depth_check, unsigned int threshold, comp cmp, thread_pool &pool) {
    if (start + 1 >= end) return;

    if (end - start <= 32) {
        insertion_sort(start, end, cmp);
        return;
    }

    if (depth_check <= 0) {
        std::make_heap(start, end, cmp);
        std::sort_heap(start, end, cmp);
        return;
    }

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1, cmp);
    iter pivot = partition(start, start, end, cmp);

    depth_check = (depth_check >> 1) + (depth_check >> 2);
    if (end - start <= threshold) {
        intro_sort(start, pivot, depth_check, cmp);
        intro_sort(pivot + 1, end, depth_check, cmp);
    } else {
        pool.register_job([start, pivot, depth_check, threshold, cmp, &pool]() {
            par_pool_sort(start, pivot, depth_check, threshold, cmp, pool); });
        par_pool_sort(pivot + 1, end, depth_check, threshold, cmp, pool);
    }
}

//start, end: [start, end) 범위의 배열에 대한 포인터
//depth check: 최초 크기는 배열 길이. 매 재귀마다 0.75배로 줄여 나감. 0이 되면 힙소트로 전환
//threshold: 싱글 스레드로 전환하는 기준 크기
//cmp: 비교 함수
template<typename iter, typename comp>
void par_sort(iter start, iter end, unsigned int depth_check, unsigned int threshold, comp cmp) {
    if (start + 1 >= end) return;

    if (end - start <= 32) {
        insertion_sort(start, end, cmp);
        return;
    }

    if (depth_check <= 0) {
        std::make_heap(start, end, cmp);
        std::sort_heap(start, end, cmp);
        return;
    }

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1, cmp);
    iter pivot = partition(start, start, end, cmp);

    depth_check = (depth_check >> 1) + (depth_check >> 2);
    if (end - start <= threshold) {
        intro_sort(start, pivot, depth_check, cmp);
        intro_sort(pivot + 1, end, depth_check, cmp);
    } else {
        std::thread t1{par_sort<iter, comp>, start, pivot, depth_check, threshold, cmp};
        par_sort(pivot + 1, end, depth_check, threshold, cmp);
        t1.join();
    }
}

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

template<typename iter, typename comp>
void sf_sort(iter start, iter end, comp cmp) {
    if (start >= end) return;

    unsigned int size = end - start;

    if (size < 10000)
        return intro_sort(start, end, end - start, cmp);

    unsigned int core_count = std::thread::hardware_concurrency();

    if (size >= 100'000'000)
        cpu_gpu_sort(&*start, &*end, core_count);

    par_sort(start, end, size, size / core_count, cmp);
}

#endif