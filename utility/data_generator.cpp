
#include "data_generator.h"

data_generator::data_generator(long long s) : size(s), num_uni(-s, s), idx_uni(0, s - 1) {
    data = new int[size];
}


data_generator::~data_generator() {
    delete[] data;
}


long long data_generator::get_size() const {
    return size;
}


int *data_generator::get_data() {
    return data;
}


void data_generator::set_size(long long s) {
    delete[] data;
    size = s;
    data = new int[size];
    num_uni.param(std::uniform_int_distribution<int>::param_type(-s, s));
    idx_uni.param(std::uniform_int_distribution<int>::param_type(0, s - 1));
}


void data_generator::random_swap() {
    int s = std::max(size / 5, 1ll);
    int v1, v2;
    for (int i = 0; i < s; i++) {
        v1 = idx_uni(gen);
        v2 = idx_uni(gen);
        std::swap(data[v1], data[v2]);
    }
}


void data_generator::set_data_by_type(int type) {
    switch (type) {
        case 1:
            make_all_rand();
            break;
        case 2:
            make_weak_rand();
            break;
        case 3:
            make_weak_rand_rev();
            break;
        case 4:
            make_sort();
            break;
        case 5:
            make_sort_rev();
            break;
        case 6:
            make_all_same();
            break;
        default:
            break;
    }
}


std::string_view data_generator::get_data_name(int type) {
    switch (type) {
        case 1:
            return "all random";
        case 2:
            return "weak random";
        case 3:
            return "reverse weak random";
        case 4:
            return "sorted";
        case 5:
            return "reverse sorted";
        case 6:
            return "all same";
        default:
            return "wrong type";
    }
}


void data_generator::make_all_rand() {
    for (int i = 0; i < size; ++i) {
        data[i] = num_uni(gen);
    }
}


void data_generator::make_sort() {
    make_all_rand();
    std::sort(std::execution::par_unseq, data, data + size);
}


void data_generator::make_sort_rev() {
    make_all_rand();
    std::sort(std::execution::par_unseq, data, data + size, std::greater<>());
}


void data_generator::make_weak_rand() {
    make_sort();
    random_swap();
}


void data_generator::make_weak_rand_rev() {
    make_sort_rev();
    random_swap();
}


void data_generator::make_all_same() {
    int v = num_uni(gen);
    std::fill(data, data + size, v);
}
