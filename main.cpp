#include <iostream>

//sort_algorithms 헤더에 모든 정렬 알고리즘의 전방 선언이 모여있습니다.
#include "sort_algorithms.h"
#include "data_generator.h"
#include "utility.h"

using namespace std;

//간단한 사용 예시입니다.
int main() {
    const int size = 100000000;
    data_generator *gen = new data_generator(size);
    int *data = new int[size];

    //제네레이터가 가진 데이터를 무작위로 뒤섞습니다.
    gen->make_all_rand();
    //뒤섞인 데이터를 임시 배열에 복사합니다.
    memcpy(data, gen->get_data(), size * sizeof(int));

    //get_proc_time 함수를 이용하면 함수 실행 시간을 쉽게 구할 수 있습니다.
    cout << "parallel intro sort 실행 시간: " << get_pretty_time(get_proc_time(parallel_intro_sort, data, data + size)) << '\n';

    //똑같은 데이터를 다시 복사해서 STL sort의 실행 시간을 측정합니다.
    memcpy(data, gen->get_data(), size * sizeof(int));
    cout << "STL sort 실행 시간: " << get_pretty_time(get_proc_time(sort<int*>, data, data + size)) << '\n';

    delete gen;
    delete[] data;
    return 0;
}
