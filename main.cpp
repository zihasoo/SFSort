#include <iostream>
#include <random>
#include <thread>
#include <future>
#include <chrono>
#include <execution>

using namespace std;
using namespace chrono;
using hc = chrono::high_resolution_clock;

//template<typename Riter>
//Riter insertion_sort(Riter left, Riter right);
//
//template<typename Riter>
//Riter bitonic_sort(Riter left, Riter right);

template<typename Riter>
Riter sorting(Riter pivot, Riter left, Riter right) {
    while (true) {
        while (left <= right && *left < *pivot) left++;
        while (left <= right && !(*right < *pivot)) right--;
        if (right < left) {
            swap(*right, *pivot);
            return right;
        }
        swap(*right, *left);
    }
}

template<typename Riter>
void p_quick_sort_step(Riter start, Riter end, unsigned int depth, unsigned int core_count) {
    end--;
    if (start >= end) {
        return;
    }
    Riter pivot = sorting(start, start + 1, end);
    if ((1 << depth) > core_count) {
        p_quick_sort_step(start, pivot, depth, core_count);
        p_quick_sort_step(pivot + 1, ++end, depth, core_count);
    }
    else {
        auto left_future = async(std::launch::async, p_quick_sort_step<Riter>, start, pivot, depth + 1, core_count);
        p_quick_sort_step(pivot + 1, ++end, depth + 1, core_count);
        left_future.wait();
    }
}

template<typename Riter>
void ziha_sort(Riter start, Riter end) {
    if (start >= end) {
        return ;
    }
    unsigned int core_count = thread::hardware_concurrency();
    unsigned int size = end - start;
//    if (size <= 16) {
//        insertion_sort(start, end);
//    }
//    else if (size >= 2'000'000) {
//        bitonic_sort(start, end);
//    }
//    else
    {
        p_quick_sort_step(start, end, 1, core_count);
    }
}

const int n = 80'000;
int nums[n];
int ran[n];

template<typename Riter>
inline void randomization(Riter begin, Riter end){
    static mt19937 gen(15);
    static uniform_int_distribution<int> uni(-n, n);
    for (;begin != end; begin++){
        *begin = uni(gen);
    }
}

int main() {
    randomization(ran, ran + n);
    copy(ran, ran + n, nums);
    auto start = hc::now();
    sort(nums, nums + n);
    auto end = hc::now();
    cout << duration_cast<microseconds>(end - start) << '\n';

    copy(ran, ran + n, nums);
    start = hc::now();
    ziha_sort(nums, nums + n);
    end = hc::now();
    cout << duration_cast<microseconds>(end - start) << '\n';
    return 0;
}
