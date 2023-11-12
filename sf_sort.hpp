#include <thread>
#include <future>
#include <utility>
#include <execution>

template<typename iter, typename comp>
void insertion_sort(iter left, iter right, comp cmp) {
    iter i, j;
    for (i = left + 1; i != right; ++i) {
        auto key(std::move(*i));
        for (j = i - 1; j >= left && cmp(key, *j); --j) {
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
iter quick_sort_partition(iter pivot, iter left, iter right, comp cmp) {
    while (true) {
        while (left <= right && cmp(*left, *pivot)) left++;
        while (left <= right && !cmp(*right, *pivot)) right--;
        if (right < left) {
            std::iter_swap(right, pivot);
            return right;
        }
        std::iter_swap(right, left);
    }
}

template<typename iter, typename comp>
void quick_sort(iter start, iter end, comp cmp) {
    end--;
    if (start >= end) return;

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1, cmp);
    iter pivot = quick_sort_partition(start, start + 1, end, cmp);

    quick_sort(start, pivot, cmp);
    quick_sort(pivot + 1, ++end, cmp);
}

template<typename iter, typename comp>
void par_quick_sort(iter start, iter end, unsigned int depth, unsigned int core_count, comp cmp) {
    end--;
    if (start >= end) return;

    iter mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end, cmp);
    iter pivot = quick_sort_partition(start, start + 1, end - 1, cmp);

    if ((1 << depth) > core_count) {
        par_quick_sort(start, pivot, depth, core_count, cmp);
        par_quick_sort(pivot + 1, ++end, depth, core_count, cmp);
    } else {
        auto left_future = async(std::launch::async, par_quick_sort<iter, comp>, start, pivot, depth + 1, core_count, cmp);
        par_quick_sort(pivot + 1, ++end, depth + 1, core_count, cmp);
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
