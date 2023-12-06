#ifndef SFSORT_DATA_GENERATOR_HPP
#define SFSORT_DATA_GENERATOR_HPP

#include <random>
#include <vector>
#include <algorithm>
#include <utility>
#include <string_view>

template<typename T>
class data_generator {
    std::vector<T> origin;
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> num_uni;
    std::uniform_int_distribution<int> idx_uni;

    void random_swap() {
        int s = std::max(origin.size() / 5, 1ull);
        int v1, v2;
        for (int i = 0; i < s; i++) {
            v1 = idx_uni(gen);
            v2 = idx_uni(gen);
            std::swap(origin[v1], origin[v2]);
        }
    }

public:
    data_generator() = default;

    explicit data_generator(int s) : origin(s), num_uni(-s, s), idx_uni(0, s - 1) {}

    void set_data_by_type(int type) {
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

    std::string_view get_data_name(int type) {
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

    void set_size(int s) {
        origin.resize(s);
        num_uni.param(std::uniform_int<int>::param_type(-s, s));
        idx_uni.param(std::uniform_int<int>::param_type(0, s - 1));
    }

    std::vector<T>& get_data() {
        return origin;
    }

    void make_all_rand() {
        for (T &x: origin) {
            x = num_uni(gen);
        }
    }

    void make_sort() {
        make_all_rand();
        std::sort(origin.begin(), origin.end());
    }

    void make_sort_rev() {
        make_all_rand();
        std::sort(origin.begin(), origin.end(), std::greater<T>());
    }

    void make_weak_rand() {
        make_sort();
        random_swap();
    }

    void make_weak_rand_rev() {
        make_sort_rev();
        random_swap();
    }

    void make_all_same() {
        int v = num_uni(gen);
        std::fill(origin.begin(), origin.end(), v);
    }
};


#endif //SFSORT_DATA_GENERATOR_HPP
