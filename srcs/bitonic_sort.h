
#ifndef SFSORT_BITONIC_SORT_H
#define SFSORT_BITONIC_SORT_H

#define THREADS (1 << 10) // 2^10
#define BLOCKS (1 << 18) // 2^18
#define NUM_VALS THREADS*BLOCKS

void bitonic_sort(int* begin, int* end);

#endif //SFSORT_BITONIC_SORT_H
