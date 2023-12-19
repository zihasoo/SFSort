#ifndef SFSORT_DATA_GENERATOR_H
#define SFSORT_DATA_GENERATOR_H

#include <random>
#include <algorithm>
#include <utility>
#include <execution>

class data_generator {
    int* data;
    long long size;
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> num_uni;
    std::uniform_int_distribution<int> idx_uni;

    void random_swap();

public:
    explicit data_generator(long long s);

    ~data_generator();

    long long get_size() const;

    int* get_data();

    void set_size(long long s);

    void set_data_by_type(int type);

    std::string_view get_data_name(int type);

    void make_all_rand();

    void make_sort();

    void make_sort_rev();

    void make_weak_rand();

    void make_weak_rand_rev();

    void make_all_same();
};


#endif //SFSORT_DATA_GENERATOR_HPP
