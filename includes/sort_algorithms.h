#ifndef SF_SORT_H
#define SF_SORT_H

// 정렬 알고리즘의 전방 선언이 모여있는 헤더 파일입니다.

// 모든 함수는 [start, end) 범위에 대해 동작합니다.
// 즉 end는 배열의 마지막 항 다음 항을 가리키는 포인터입니다.
// sort(start, start + size) 형태로 사용하기 위해 이러한 범위를 인자로 받습니다.

// 삽입 정렬
// 데이터 크기가 충분히 작을 때 재귀호출을 하는 정렬 알고리즘보다 빠릅니다.
// 또한 데이터가 거의 정렬되어 있다면 O(n)에 근접하는 시간 복잡도를 보여줍니다.
void insertion_sort(int *start, int *end);

// 인트로 정렬
// 재귀 깊이가 깊어지면 힙정렬로 전환하고, 분할된 크기가 너무 작으면 삽입 정렬로 전환합니다.
// 이 덕분에 퀵정렬의 단점을 극복하고 대부분의 상황에서 매우 빠른 모습을 보여줍니다
void intro_sort(int *start, int *end);

// 병렬 인트로 정렬
// 인트로 정렬을 멀티스레드로 실행합니다.
// 내부적으로 threshold인자를 통해 분할된 크기가 threshold보다 작아지면 싱글 스레드로 전환합니다.
void parallel_intro_sort(int *start, int *end);

// 스레드 풀 기반 병렬 인트로 정렬
// 인트로 정렬을 스레드 풀 기반으로 실행합니다.
// 위의 병렬 인트로 정렬과 거의 똑같으나 스레드 풀을 사용한다는 점만 다릅니다.
void thread_pool_parallel_intro_sort(int *start, int *end);


#endif