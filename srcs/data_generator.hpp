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
    std::mt19937 gen{15};
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
                all_rand();
                break;
            case 2:
                weak_rand();
                break;
            case 3:
                weak_rand_rev();
                break;
            case 4:
                sort();
                break;
            case 5:
                sort_rev();
                break;
            case 6:
                all_same();
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

    std::vector<T> get_data() {
        return origin;
    }

    void all_rand() {
        for (T &x: origin) {
            x = num_uni(gen);
        }
    }

    void sort() {
        all_rand();
        std::sort(origin.begin(), origin.end());
    }

    void sort_rev() {
        all_rand();
        std::sort(origin.begin(), origin.end(), std::greater<T>());
    }

    void weak_rand() {
        sort();
        random_swap();
    }

    void weak_rand_rev() {
        sort_rev();
        random_swap();
    }

    void all_same() {
        int v = num_uni(gen);
        std::fill(origin.begin(), origin.end(), v);
    }
};


#endif //SFSORT_DATA_GENERATOR_HPP
