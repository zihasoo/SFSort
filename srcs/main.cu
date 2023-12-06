#include <iostream>
#include <execution>
#include <cmath>

#include "thread_pool.h"
#include "sf_sort.h"
#include "data_generator.hpp"
#include "utility.h"

using namespace std;
using i64 = long long;

unsigned core_cnt = thread::hardware_concurrency();
data_generator<int> gen;

void benchmark_by_data_size() {
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"sf",   [&data]() {
                sf_sort(data.begin(), data.end(), less<>{});
            }},
//            {"pool", [&data]() {
//                thread_pool pool{core_cnt};
//                par_pool_sort(data.begin(), data.end(), data.size(), data.size() / core_cnt, less<>{}, pool);
//            }},
//            {"par",  [&data]() {
//                sort(execution::par_unseq, data.begin(), data.end());
//            }},
            {"seq",  [&data]() {
                sort(data.begin(), data.end());
            }},
    };

    cout << "     size | ";
    for (auto &func: funcs)
        cout << func.first << " | ";
    cout << '\n';
    for (int size = 2; size < 400'000'000; size += (size >> 1)) {
        gen.set_size(size);
        cout.width(9);
        cout << size << " | ";
        vector<i64> sums(funcs.size(), 0);
        int it_cnt = 3, type_cnt = 6;
        for (int i = 0; i < it_cnt; i++) {
            for (int t = 1; t <= type_cnt; t++) {
                gen.set_data_by_type(t);
                for (int j = 0; j < funcs.size(); j++) {
                    data = gen.get_data();
                    sums[j] += get_proc_time(funcs[j].second);
                }
            }
        }
        for (i64 s : sums) {
            cout << get_pretty_time(s / (it_cnt * type_cnt)) << " | ";
        }
        cout << '\n';
    }
}

void benchmark_by_data_type() {
    gen.set_size(1'000'000'000);
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"seq",     [&data]() {
                intro_sort(data.begin(), data.end(), data.size(), less<>{});
            }},
            {"no pool", [&data]() {
                par_sort(data.begin(), data.end(), data.size(), data.size() / core_cnt,
                         less<>{});
            }},
            {"pool",    [&data]() {
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
        int iter_cnt = 3;
        vector<i64> sums(funcs.size(), 0);
        for (int i = 0; i < iter_cnt; i++) {
            gen.set_data_by_type(t);
            for (int j = 0; j < funcs.size(); j++) {
                data = gen.get_data();
                sums[j] += get_proc_time(funcs[j].second);
            }
        }
        for (i64 s: sums) {
            cout << get_pretty_time(s / iter_cnt) << " | ";
        }
        cout << '\n';
    }
}


int main() {
    benchmark_by_data_size();
    benchmark_by_data_type();

    return 0;
}
