#include "utility.h"

using namespace std;

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

void pretty_print_time(long long c) {
    cout.precision(2);
    if (c >= 1000ll * 1000 * 1000) {
        cout << fixed << (c / (1000ll * 1000)) / 1000.0 << "s";
    } else if (c >= 1000ll * 1000) {
        cout << fixed << (c / 1000l) / 1000.0 << "ms";
    } else if (c >= 1000ll) {
        cout << fixed << c / 1000.0 << "us";
    } else {
        cout << fixed << c << "ns";
    }
}
