#include "utility.h"

bool confirm_sorted(int* begin, int* end) {
    int* before = begin;
    for (int* cur = begin + 1; cur != end; cur++) {
        if (*cur < *before) return false;
        before = cur;
    }
    return true;
}

std::string get_pretty_time(long long c) {
    std::stringstream ss;
    if (c >= 1000ll * 1000 * 1000) {
        ss << (c / (1000ll * 1000)) / 1000.0 << "s";
    } else if (c >= 1000ll * 1000) {
        ss << (c / 1000l) / 1000.0 << "ms";
    } else if (c >= 1000ll) {
        ss << c / 1000.0 << "us";
    } else {
        ss << c << "ns";
    }
    return ss.str();
}
