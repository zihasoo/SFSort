
#include "sort_algorithms.h"

void insertion_sort(int* start, int* end) {
    int* i, *j;
    if (start == end) return;
    for (i = start + 1; i != end; ++i) {
        auto key = *i;
        for (j = i - 1; j >= start && key < *j; --j) {
            *(j + 1) = *j;
        }
        *(j + 1) = key;
    }
}
