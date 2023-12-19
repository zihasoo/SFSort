
#include "intro_sort_utility.h"

void set_pivot(int *dest, int *a, int *b, int *c) {
    if (*a < *b) {
        if (*b < *c)
            std::iter_swap(dest, b);
        else if (*a < *c)
            std::iter_swap(dest, c);
        else
            std::iter_swap(dest, a);
    } else if (*a < *c)
        std::iter_swap(dest, a);
    else if (*b < *c)
        std::iter_swap(dest, c);
    else
        std::iter_swap(dest, b);
}

int *partition(int *pivot, int *start, int *end) {
    while (true) {
        do start++;
        while (*start < *pivot);
        do end--;
        while (*pivot < *end);
        if (end <= start) {
            std::iter_swap(end, pivot);
            return end;
        }
        std::iter_swap(end, start);
    }
}
