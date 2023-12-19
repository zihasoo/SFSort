
#ifndef SFSORT_INTRO_SORT_UTILITY_H
#define SFSORT_INTRO_SORT_UTILITY_H

#include <algorithm>

// 인트로 정렬 관련 함수들이 공통으로 사용하는 함수가 모여있는 헤더 파일입니다.

//depth check를 인자로 받는 원본 intro sort 함수가 다른 병렬 인트로 정렬에 필요하기 때문에 전방 선언합니다
void __intro_sort(int *start, int *end, unsigned int depth_check);

// median of three 방법으로 피봇을 설정하는 함수입니다.
// a, b, c에 값을 넣어 주면, 이들 중 중앙값을 dest와 스왑합니다.
// dest는 피봇으로 설정하고 싶은 위치를 넣어주면 됩니다.
void set_pivot(int *dest, int *a, int *b, int *c);

// hoare partitioning 방법으로 pivot기준에 맞춰 start부터 end까지 파티셔닝하는 함수입니다.
int *partition(int *pivot, int *start, int *end);

#endif //SFSORT_INTRO_SORT_UTILITY_H
