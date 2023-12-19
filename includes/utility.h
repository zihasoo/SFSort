#ifndef SFSORT_UTILITY_H
#define SFSORT_UTILITY_H

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <utility>

// 배열이 정렬되었는지 확인하는 함수입니다.
bool confirm_sorted(int* begin, int* end);

// 나노초를 나타내는 수를 인자로 받아 적절한 크기의 단위로 바꿔서 출력해줍니다.
// 예를 들어 인자로 1003이 들어오면, "1.003us" 를 반환합니다
std::string get_pretty_time(long long c);

//인자로 들어온 함수를 실행하는데 걸리는 시간을 측정해주는 함수입니다.
template<typename func_t>
long long get_proc_time(func_t f) {
    auto st_t = std::chrono::high_resolution_clock::now();
    f();
    auto ed_t = std::chrono::high_resolution_clock::now();
    return (ed_t - st_t).count();
}

//위와 똑같은데 임의의 개수의 인자를 받아서 실행할 함수에 인자로 그대로 전달해줍니다.
template<typename func_t, typename... arg_t>
long long get_proc_time(func_t f, arg_t... args) {
    auto st_t = std::chrono::high_resolution_clock::now();
    f(std::forward<arg_t>(args)...);
    auto ed_t = std::chrono::high_resolution_clock::now();
    return (ed_t - st_t).count();
}


#endif //SFSORT_UTILITY_H
