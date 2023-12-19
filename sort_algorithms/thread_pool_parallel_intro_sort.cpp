
#include "thread_pool.h"
#include "intro_sort_utility.h"
#include "sort_algorithms.h"

void __thread_pool_parallel_intro_sort(int *start, int *end, unsigned int depth_check, unsigned int threshold, thread_pool &pool);

void thread_pool_parallel_intro_sort(int *start, int *end) {
    unsigned int core_count = std::thread::hardware_concurrency();
    //스레드 풀을 생성해서 인자로 넘깁니다.
    thread_pool pool{core_count};
    __thread_pool_parallel_intro_sort(start, end, end - start, (end - start) / core_count, pool);
}

//인자는 parallel_intro_sort와 똑같고, 마지막 인자로 스레드 풀을 받는 것만 다릅니다
void __thread_pool_parallel_sort(int *start, int *end, unsigned int depth_check, unsigned int threshold, thread_pool &pool) {
    //함수 내부의 설명은 intro_sort에 더 자세히 적혀 있습니다.
    if (start + 1 >= end) return;

    if (end - start <= 32) {
        insertion_sort(start, end);
        return;
    }

    if (depth_check <= 0) {
        std::make_heap(start, end);
        std::sort_heap(start, end);
        return;
    }

    int *mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1);
    int *pivot = partition(start, start, end);

    depth_check = (depth_check >> 1) + (depth_check >> 2);
    if (end - start <= threshold) {
        __intro_sort(start, pivot, depth_check);
        __intro_sort(pivot + 1, end, depth_check);
    } else {
        //절반의 데이터는 스레드풀에 넘겨서 실행하고, 절반은 재귀 호출로 실행합니다.
        pool.register_job([start, pivot, depth_check, threshold, &pool]() {
            __thread_pool_parallel_intro_sort(start, pivot, depth_check, threshold, pool);
        });
        __thread_pool_parallel_intro_sort(pivot + 1, end, depth_check, threshold, pool);
    }
}