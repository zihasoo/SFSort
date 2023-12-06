#include "utility.h"

using namespace std;

//벡터 출력
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

std::string get_pretty_time(long long c) {
    std::stringstream ss;
    if (c >= 1000ll * 1000 * 1000) {
        ss << fixed << (c / (1000ll * 1000)) / 1000.0 << "s";
    } else if (c >= 1000ll * 1000) {
        ss << fixed << (c / 1000l) / 1000.0 << "ms";
    } else if (c >= 1000ll) {
        ss << fixed << c / 1000.0 << "us";
    } else {
        ss << fixed << c << "ns";
    }
    return ss.str();
}
