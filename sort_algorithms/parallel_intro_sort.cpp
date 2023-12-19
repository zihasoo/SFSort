
#include "sort_algorithms.h"
#include "intro_sort_utility.h"

#include <thread>

static void __parallel_intro_sort(int* start, int* end, unsigned int depth_check, unsigned int threshold);

void parallel_intro_sort(int* start, int* end) {
    //hardware_concurrency 함수는 현재 프로그램이 실행중인 컴퓨터의 논리 코어 개수를 반환합니다.
    //해당 함수는 생각보다 느리기 때문에 데이터 크기가 작다면 아예 실행하지 않고 싱글 스레드 정렬 함수를 호출하는 것이 좋습니다.
    unsigned int core_count = std::thread::hardware_concurrency();
    //싱글 스레드로 전환하는 기준 크기인 threshold는 (배열 크기 / 코어 개수)로 되어 있습니다.
    __parallel_intro_sort(start, end, end - start, (end - start) / core_count);
}

//인자 설명
//start, end: [start, end) 범위의 배열에 대한 포인터입니다
//depth check: 최초 크기는 배열 길이이고, 매 재귀 호출마다 0.75배로 줄여 나갑니다. 이 크기가 0이 되면 힙소트로 전환합니다. (재귀 깊이가 1.5 * log(n)이 되었을 때)
//threshold: 싱글 스레드로 전환하는 기준 크기입니다.
static void __parallel_intro_sort(int* start, int* end, unsigned int depth_check, unsigned int threshold) {
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

    int* mid = start + (end - start) / 2;
    set_pivot(start, start + 1, mid, end - 1);
    int* pivot = partition(start, start, end);


    depth_check = (depth_check >> 1) + (depth_check >> 2);
    //현재 배열 크기가 threshold보다 작다면 싱글 스레드로 전환합니다.
    if (end - start <= threshold) {
        __intro_sort(start, pivot, depth_check);
        __intro_sort(pivot + 1, end, depth_check);
    } else {
        //std 스레드를 사용하여 절반의 데이터에 대해 함수를 실행시키고, 나머지는 절반은 재귀호출로 실행합니다.
        std::thread t1{__parallel_intro_sort, start, pivot, depth_check, threshold};
        __parallel_intro_sort(pivot + 1, end, depth_check, threshold);
        t1.join(); //조인을 해서 다른 스레드를 기다려 줍니다.
    }
}
