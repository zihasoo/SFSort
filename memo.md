출처 모음
=

[bitonic sort implementation](https://gist.github.com/mre/1392067)

[quick sort libstdc++ implementation](https://github.com/gcc-mirror/gcc/blob/d9375e490072d1aae73a93949aa158fcd2a27018/libstdc%2B%2B-v3/include/bits/stl_algo.h#L1950)

[gpu merge sort](https://github.com/rbga/CUDA-Merge-and-Bitonic-Sort/tree/master)

[parallel quick sort (java)](https://github.com/kdgyun/Sorting_Algorithm/blob/master/SortingAlgorithm/Java/ParallelSort/ParallelLeftPivotQuickSort.java)

[parallel intro sort (go)](https://github.com/kdgyun/GoSortingAlgorithms/blob/main/sorts/parallel_intro_sort.go)

[heap sort](https://github.com/ArminGh02/sorting-algorithms/blob/main/include/sorting_algorithms/sort.hpp#L193)

[thread pool 구현](https://modoocode.com/285)

테스트 데이터 종류:
1. 완전히 섞인 데이터
2. 약간 정렬되어 있는 데이터
3. 약간 역순으로 정렬되어 있는 데이터
4. 정렬되어 있는 데이터
5. 역순으로 정렬되어 있는 데이터
6. 모든 데이터의 값이 같은 데이터

고려해야 하는 것:   
- 병렬 힙 정렬이 효율적인지, 가능한지 알아보기   
  -> 그닥 효율적이지 않은 것 같음
- 코어 수에 따라 언제 병렬로 전환해야 하는지 판단 기준 세우기    
  -> std sort는 어떻게 이 기준을 잡았는지 알아내기
- cudamemcpy 시간 측정하기
- GPU메모리 크기를 알아낼 수 있는지 알아보기 (cudamalloc 실패때문에)   
- 공유 GPU메모리에 cudamalloc하는 법 알아내기

스레드 생성 시간: 15 ~ 25 마이크로초
스레드 생성 및 조인 시간: 100 ~ 150 마이크로초