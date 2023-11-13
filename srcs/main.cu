#include <iostream>
#include <execution>
#include <chrono>
#include <utility>

#include "sf_sort.hpp"
#include "data_generator.hpp"

using namespace std;
using namespace chrono;
using hc = chrono::high_resolution_clock;

const int n = 5000000;
data_generator<int> generator{n};

ostream &operator<<(ostream &out, const vector<int> &v) {
    if (v.empty()) {
        return out << "[]";
    }
    cout << '[' << v[0];
    for (int i = 1; i < v.size(); i++) {
        out << ", " << v[i];
    }
    cout << ']';
    return out;
}

void time_printer(auto d) {
    auto c = d.count();
    if (c >= 1000ll * 1000 * 1000) {
        cout << duration_cast<milliseconds>(d).count() / 1000.0 << "s\n";
    } else if (c >= 1000ll * 1000) {
        cout << duration_cast<microseconds>(d).count() / 1000.0 << "ms\n";
    } else if (c >= 1000ll) {
        cout << c / 1000.0 << "us\n";
    } else {
        cout << c << "ns\n";
    }
}

template<typename func_t>
void print_proc_time(func_t f) {
    auto st_t = hc::now();
    f();
    auto ed_t = hc::now();
    time_printer(ed_t - st_t);
}

template<typename func_t, typename... arg_t>
void print_proc_time(func_t f, arg_t... args) {
    auto st_t = hc::now();
    f(forward<arg_t>(args)...);
    auto ed_t = hc::now();
    time_printer(ed_t - st_t);
}

void test_funcs() {
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"intro sort",[&data](){
                intro_sort(data.begin(), data.end(), data.end() - data.begin(), less<>());
            }},
            {"stl sort", [&data]() {
                sort(data.begin(), data.end());
            }},
            {"par stl sort", [&data]() {
                sort(execution::par_unseq,data.begin(), data.end());
            }}
    };

    for (int s = 16; s < 100'000; s *= 2) {

        for (int i = 1; i <= 6; i++) {
            generator.set_data_by_type(i);
            cout << generator.get_data_name(i) << ":\n";
            for (auto &func: funcs) {
                cout << func.first << ": ";
                data = generator.get_data();
                print_proc_time(func.second);
            }
            cout << '\n';
        }
    }
}

int main() {
    test_funcs();

    return 0;
}
