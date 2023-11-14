#ifndef SF_SORT_HPP
#define SF_SORT_HPP

#include <thread>
#include <future>
#include <utility>
#include <execution>
#include <algorithm>

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

template<typename iter, typename comp>
void par_intro_sort(iter start, iter end, unsigned int depth_check, unsigned int threshold, comp cmp, thread_pool &tp) {
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
        tp.register_job([start, pivot, depth_check, threshold, cmp, &tp]() { par_intro_sort(start, pivot, depth_check, threshold, cmp, tp); });
        par_intro_sort(pivot + 1, end, depth_check, threshold, cmp, tp);
    }
}

template<typename iter, typename comp>
void sf_sort(iter start, iter end, comp cmp) {
    if (start >= end) return;

    unsigned int core_count = std::thread::hardware_concurrency();
    unsigned int size = end - start;

    if (size >= 2'000'000) {
        bitonic_sort(start, end, cmp);
    } else {
        par_intro_sort(start, end, 1, core_count, cmp);
    }
}

#endif