#include <iostream>
#include <execution>

#include "thread_pool.h"
#include "sf_sort.hpp"
#include "data_generator.hpp"
#include "utility.h"
#include "bitonic_sort.h"

using namespace std;
using ll = long long;

data_generator<int> gen;

void  benchmark_by_data_size() {
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"seq", [&data]() {
                sort(data.begin(), data.end());
            }},
            {"par", [&data]() {
                sort(execution::par_unseq, data.begin(), data.end());
            }},
            {"my",  [&data]() {
                unsigned x = std::thread::hardware_concurrency();
                thread_pool tp{x};
                par_intro_sort(data.begin(), data.end(), data.size(), data.size() / x, less<>{}, tp);
            }},
    };

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
            int it = 3, types = 1;
            for (int i = 0; i < it; i++) {
                for (int t = 1; t <= types; t++) {
                    gen.set_data_by_type(t);
                    data = gen.get_data();
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
    gen.set_size(2'000'000);
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"seq", [&data]() {
                sort(data.begin(), data.end());
            }},
            {"par", [&data]() {
                sort(execution::par_unseq, data.begin(), data.end());
            }},
            {"my",  [&data]() {
                unsigned x = std::thread::hardware_concurrency();
                thread_pool tp{x};
                par_intro_sort(data.begin(), data.end(), data.size(), data.size() / x, less<>{}, tp);
            }},
    };

    cout.width(20);
    for (auto &func: funcs) {
        cout << func.first << " | ";
        cout.width(8);
    }
    cout << '\n';
    for (int t = 1; t <= 6; t++) {
        cout.width(20);
        cout << gen.get_data_name(t) << " | ";
        for (auto &func: funcs) {
            ll sum = 0;
            int it = 3;
            for (int i = 0; i < it; i++) {
                gen.set_data_by_type(t);
                data = gen.get_data();
                sum += get_proc_time(func.second);
            }
            pretty_print_time(sum / it);
            cout << " | ";
        }
        cout << '\n';
    }
}

int main() {
    benchmark_by_data_size();

    return 0;
}
