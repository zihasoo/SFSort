#include <iostream>
#include <execution>
#include <chrono>

#include "sf_sort.hpp"
#include "data_generator.hpp"

using namespace std;
using namespace chrono;
using hc = chrono::high_resolution_clock;

const int n = 50000;
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
    vector<function<void(void)>> funcs = {
            []() {
                vector<int> data = generator.get_data();
                quick_sort(data.begin(), data.end(), data.size(), less<>());
            },
            []() {
                vector<int> data = generator.get_data();
                sort(data.begin(), data.end());
            }
    };

    vector<string_view> func_names = {"intro sort", "stl sort"};

    for (int i = 1; i <= 6; i++) {
        generator.set_data_by_type(i);
        cout << generator.get_data_name(i) << ":\n";
        for (int j = 0; j < funcs.size(); j++) {
            cout << func_names[j] << ": ";
            print_proc_time(funcs[j]);
        }
        cout << '\n';
    }
}

int main() {
    test_funcs();

    return 0;
}
