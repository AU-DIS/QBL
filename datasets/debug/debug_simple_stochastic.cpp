

#include "debug_simple_stochastic.h"
#include "random"
#include "iostream"
#include "../../utilities/random_gen.h"

/// This debug data set is stochastic and contains distributes a single reward across
/// all the choices. Each choice is given a single static reward from a gaussian distribution
/// with mean 10 and stddev 40. The values are range normalized to [0, 1].
debug_simple_stochastic::debug_simple_stochastic(int K) {
    k = K;

    // Modify as needed
    constexpr int MIN = 0;
    constexpr int MAX = 1;

    std::mt19937 gen = random_gen();
    std::normal_distribution<double> d{1,2};

    _data_matrix.reserve(k);
    for (int i = 0; i < k; i++) {
        double v = d(gen);
        _data_matrix.push_back(v);
    }


    auto max_element = *std::max_element(_data_matrix.begin(), _data_matrix.end());
    auto min_element = *std::min_element(_data_matrix.begin(), _data_matrix.end());
    for (auto &v : _data_matrix) v = (v-min_element)/(max_element - min_element);


    double mean = 0;
    for (auto v : _data_matrix) mean += v;
    mean_value =  mean/(_data_matrix.size());
    std::cout << mean_value;
}

double debug_simple_stochastic::feedback(int choice, double &regret) {
    regret = 1 - _data_matrix[choice];
    return _data_matrix[choice];
}