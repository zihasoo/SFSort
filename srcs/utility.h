#ifndef SFSORT_UTILITY_H
#define SFSORT_UTILITY_H

#include <iostream>
#include <chrono>
#include <utility>
#include <vector>

std::ostream &operator<<(std::ostream &out, const std::vector<int> &v);

void pretty_print_time(long long c);

template<typename iter, typename comp>
bool confirm_sorted(iter begin, iter end, comp cmp) {
    iter before = begin;
    for (iter cur = begin + 1; cur != end; cur++) {
        if (cmp(*cur, *before)) return false;
        before = cur;
    }
    return true;
}

template<typename func_t>
long long get_proc_time(func_t f) {
    auto st_t = std::chrono::high_resolution_clock::now();
    f();
    auto ed_t = std::chrono::high_resolution_clock::now();
    return (ed_t - st_t).count();
}

template<typename func_t, typename... arg_t>
long long get_proc_time(func_t f, arg_t... args) {
    auto st_t = std::chrono::high_resolution_clock::now();
    f(std::forward<arg_t>(args)...);
    auto ed_t = std::chrono::high_resolution_clock::now();
    return (ed_t - st_t).count();
}


#endif //SFSORT_UTILITY_H
