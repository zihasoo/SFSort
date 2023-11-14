#include <iostream>
#include <execution>
#include <chrono>
#include <utility>

#include "sf_sort.hpp"
#include "data_generator.hpp"

using namespace std;
using namespace chrono;
using hc = chrono::high_resolution_clock;
using ll = long long;

data_generator<int> gen;

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

void pretty_print_time(ll c) {
    if (c >= 1000ll * 1000 * 1000) {
        cout << (c / (1000ll * 1000)) / 1000.0 << "s";
    } else if (c >= 1000ll * 1000) {
        cout << (c / 1000l) / 1000.0 << "ms";
    } else if (c >= 1000ll) {
        cout << c / 1000.0 << "us";
    } else {
        cout << c << "ns";
    }
}

template<typename func_t>
ll get_proc_time(func_t f) {
    auto st_t = hc::now();
    f();
    auto ed_t = hc::now();
    return (ed_t - st_t).count();
}

template<typename func_t, typename... arg_t>
ll get_proc_time(func_t f, arg_t... args) {
    auto st_t = hc::now();
    f(std::forward<arg_t>(args)...);
    auto ed_t = hc::now();
    return (ed_t - st_t).count();
}

void test_funcs() {
    vector<int> data;
    vector<pair<string_view, function<void(void)>>> funcs = {
            {"seq", [&data]() {
                sort(data.begin(), data.end());
            }},
            {"par", [&data]() {
                sort(execution::par_unseq, data.begin(), data.end());
            }},
    };

    cout << "     size | ";
    for (auto &func: funcs)
        cout << func.first << " | ";
    cout << '\n';
    for (int s = 2; s < 100'000'000; s += (s >> 1)) {
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

void test() {
    gen.set_size(20'000'000);
    ll sum = 0;
    cout << "intro sort\n";
    for (int t = 1; t <= 6; t++) {
        gen.set_data_by_type(t);
        auto& data = gen.get_data_ref();
        ll a = get_proc_time(intro_sort<vector<int>::iterator, less<>>, data.begin(), data.end(), data.size(),
                             less<>{});
        sum += a;
        cout << gen.get_data_name(t) << ": ";
        pretty_print_time(a);
        cout << '\n';
    }
    cout << "avg: ";
    pretty_print_time(sum / 6);
    cout << "\n\n";

    sum = 0;
    cout << "stl sort\n";
    for (int t = 1; t <= 6; t++) {
        gen.set_data_by_type(t);
        auto& data = gen.get_data_ref();
        ll a = get_proc_time(sort<vector<int>::iterator>, data.begin(), data.end());
        sum += a;
        cout << gen.get_data_name(t) << ": ";
        pretty_print_time(a);
        cout << '\n';
    }
    cout << "avg: ";
    pretty_print_time(sum / 6);
    cout << "\n\n";
}

int main() {
    test_funcs();
    //test();

    return 0;
}
