#include <thread>
#include <future>
#include <utility>
#include <execution>
#include <algorithm>

#include "thread_pool.h"

template<typename iter, typename comp>
void insertion_sort(iter start, iter end, comp cmp) {
    iter i, j;
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
iter quick_sort_partition(iter pivot, iter start, iter end, comp cmp) {
    while (true) {
        while (start <= end && cmp(*start, *pivot)) start++;
        while (start <= end && !cmp(*end, *pivot)) end--;
        if (end < start) {
            std::iter_swap(end, pivot);
            return end;
        }
        std::iter_swap(end, start);
    }
}

template<typename iter, typename comp>
void quick_sort(iter start, iter end, int depth_check, comp cmp) {
    if (start >= end) return;
    if (start + 1 >= end) return;

    if (depth_check <= 0) {
        std::make_heap(start, end, cmp);
        std::sort_heap(start, end, cmp);
        return ;
    }

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1, cmp);
    iter pivot = quick_sort_partition(start, start + 1, end - 1, cmp);

    depth_check = (depth_check >> 1) + (depth_check >> 2);
    quick_sort(start, pivot, depth_check, cmp);
    quick_sort(pivot + 1, end, depth_check, cmp);
}

template<typename iter, typename comp>
void par_quick_sort(iter start, iter end, int depth_check, comp cmp) {
    end--;
    if (start >= end) return;

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end, cmp);
    iter pivot = quick_sort_partition(start, start + 1, end - 1, cmp);

    depth_check = (depth_check >> 1) + (depth_check >> 2);
    if (end - start < 1000) {
        par_quick_sort(start, pivot, depth_check, cmp);
        par_quick_sort(pivot + 1, ++end, depth_check, cmp);
    } else {
        auto left_future = async(std::launch::async, par_quick_sort<iter, comp>, start, pivot,depth_check, cmp);
        par_quick_sort(pivot + 1, ++end, depth_check, cmp);
        left_future.wait();
    }
}

template<typename iter, typename comp>
void sf_sort(iter start, iter end, comp cmp) {
    if (start >= end) return;

    unsigned int core_count = std::thread::hardware_concurrency();
    unsigned int size = end - start;

    if (size <= 16) {
        insertion_sort(start, end, cmp);
    } else if (size >= 2'000'000) {
        bitonic_sort(start, end, cmp);
    } else {
        par_quick_sort(start, end, 1, core_count, cmp);
    }
}
