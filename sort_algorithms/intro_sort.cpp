
#include "sort_algorithms.h"
#include "intro_sort_utility.h"

void intro_sort(int *start, int *end) {
    __intro_sort(start, end, end - start);
}

//인자 설명
//start, end: [start, end) 범위의 배열에 대한 포인터입니다
//depth check: 최초 크기는 배열 길이이고, 매 재귀 호출마다 0.75배로 줄여 나갑니다. 이 크기가 0이 되면 힙소트로 전환합니다. (재귀 깊이가 1.5 * log(n)이 되었을 때)
void __intro_sort(int *start, int *end, unsigned int depth_check) {
    if (start + 1 >= end) return;

    //분할된 크기가 32보다 작다면 삽입 정렬로 마무리합니다.
    if (end - start <= 32) {
        insertion_sort(start, end);
        return;
    }

    //재귀 깊이가 1.5 * log(n)이상이 되면 힙 정렬로 마무리합니다.
    if (depth_check <= 0) {
        //STL의 heap관련 함수를 이용했습니다.
        std::make_heap(start, end);
        std::sort_heap(start, end);
        return;
    }

    int *mid = start + (end - start) / 2;
    //set_pivot 함수로 피봇을 설정합니다.
    set_pivot(start, start + 1, mid, end - 1);
    //partition 함수로 위에서 설정한 피봇을 기준으로 파티셔닝합니다.
    int *pivot = partition(start, start, end);

    //depth_check는 depth_check / 2 + depth_check / 4로 줄어들고, 이는 0.5 + 0.25로 0.75배가 됩니다
    depth_check = (depth_check >> 1) + (depth_check >> 2);
    //피봇 기준으로 절반씩 재귀 호출합니다.
    __intro_sort(start, pivot, depth_check);
    __intro_sort(pivot + 1, end, depth_check);
}

