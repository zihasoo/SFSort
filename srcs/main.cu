#include <iostream>
#include <execution>
#include <cmath>

#include "thread_pool.h"
#include "sf_sort.hpp"
#include "data_generator.hpp"
#include "utility.h"
#include "bitonic_sort.h"

using namespace std;
using ll = long long;

unsigned core_cnt = thread::hardware_concurrency();
data_generator<int> gen;

//TODO 각 함수가 똑같은 데이터로 실행되도록 수정 해야함
void benchmark_by_data_size() {
    vector<int> &data = gen.get_data();
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"my",   [&data]() {
                par_sort(data.begin(), data.end(), data.size(), data.size() / core_cnt,
                         less<>{});
            }},
            {"pool", [&data]() {
                thread_pool pool{core_cnt};
                par_pool_sort(data.begin(), data.end(), data.size(), data.size() / core_cnt, less<>{}, pool);
            }},
            {"par",  [&data]() {
                sort(execution::par_unseq, data.begin(), data.end());
            }},
            {"seq",  [&data]() {
                sort(data.begin(), data.end());
            }},
    };
    vector<int> result(6, 0);
    cout << "     size | ";
    for (auto &func: funcs)
        cout << func.first << " | ";
    cout << '\n';
    for (int s = 2; s < 40'000'000; s += (s >> 1)) {
        gen.set_size(s);
        cout.width(9);
        cout << s << " | ";
        for (auto &func: funcs) {
            ll sum = 0;
            int it = 3, types = 6;
            for (int i = 0; i < it; i++) {
                for (int t = 1; t <= types; t++) {
                    gen.set_data_by_type(t);
                    sum += get_proc_time(func.second);
                }
            }
            pretty_print_time(sum / it * types);
            cout << " | ";
        }
        cout << '\n';
    }
}

void benchmark_by_data_type() {
    gen.set_size(1'000'000'000);
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"seq", [&data]() {
                intro_sort(data.begin(), data.end(), data.size(), less<>{});
            }},
            {"no pool", [&data]() {
                par_sort(data.begin(), data.end(), data.size(), data.size() / core_cnt,
                         less<>{});
            }},
            {"pool", [&data]() {
                thread_pool pool{core_cnt};
                par_pool_sort(data.begin(), data.end(), data.size(), data.size() / core_cnt, less<>{}, pool);
            }},
    };

    cout << "           data type | ";
    for (auto &func: funcs) {
        cout.width(8);
        cout << func.first << " | ";
    }
    cout << '\n';
    for (int t = 1; t <= 6; t++) {
        cout.width(20);
        cout << gen.get_data_name(t) << " | ";
        int it = 3;
        vector<ll> sums(funcs.size(), 0);
        for (int i = 0; i < it; i++) {
            gen.set_data_by_type(t);
            for (int j = 0; j < funcs.size(); j++) {
                data = gen.get_data();
                sums[j] += get_proc_time(funcs[j].second);
            }
        }
        for (ll s: sums) {
            pretty_print_time(s / it);
            cout << " | ";
        }
        cout << '\n';
    }
}

void temp(int *begin, int *end) {
    unsigned long long cpu_size = (end - begin) / 5 * 2;
    unsigned long long gpu_size = (end - begin) / 5 * 3 + ((end - begin) % 5);
    unsigned long long cpu_bytes = cpu_size * sizeof(int);
    unsigned long long gpu_bytes = gpu_size * sizeof(int);
    int *arr1 = new int[cpu_size];
    int *arr2;

    unsigned long long padded_size = 1;
    while (padded_size < gpu_size) {
        padded_size <<= 1;
    }
    arr2 = new int[padded_size];

    memcpy(arr1, begin, cpu_bytes);
    memcpy(arr2, begin + cpu_size, gpu_bytes);
    for(unsigned long long i = gpu_size; i < padded_size; i++) {
        arr2[i] = INT_MAX;
    }

    auto t = std::thread(bitonic_sort, arr2, arr2 + padded_size, gpu_bytes);
    par_sort(arr1, arr1 + cpu_size, cpu_size, cpu_size / core_cnt, less<>{});
    t.join();

    std::merge(arr1, arr1 + cpu_size, arr2, arr2 + gpu_size, begin);

    delete[] arr1;
    delete[] arr2;
}

int main() {
    //benchmark_by_data_size();
    //benchmark_by_data_type();
    gen.set_size(1'500'000'000);
    gen.make_all_rand();

    auto& vec = gen.get_data();
    auto res1 = get_proc_time(temp,  &vec[0], &vec[0] + vec.size());
    pretty_print_time(res1);
    cout << '\n';

    gen.make_all_rand();
    auto res2 = get_proc_time(par_sort<vector<int>::iterator, less<>>, vec.begin(), vec.end(), vec.size(), vec.size() / core_cnt, less<>{});
    pretty_print_time(res2);

    temp(&vec[0], &vec[0] + vec.size());

    return 0;
}
