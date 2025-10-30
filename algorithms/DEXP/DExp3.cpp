

#include "DExp3.h"
#include <tuple>
#include <vector>
#include "iostream"


DExp3::DExp3(size_t k, double gamma, double eta) : _weights(k), _k(k), _gamma(gamma), _eta(eta) {
    for (int i = 0; i < _k; i++) {
        _weights.push_back(1.0/_k);
    }
}

Dexp3_return_values DExp3::choose() {

    std::discrete_distribution<size_t> d(_weights.begin(), _weights.end());
    int choice1 = d(_random_gen);
    int choice2 = d(_random_gen);
    return Dexp3_return_values{choice1, choice2};
}

void DExp3::give_reward(size_t x, size_t y, size_t winner, int round) {

    std::vector<double> s_tilde;
    s_tilde.reserve(_k);

    for (int i = 0; i < _k; i++) {
        double j_sum = 0;
        for (int j = 0; j < _k; j++) {
            j_sum += ((y == j ? 1 : 0) * winner)/_weights[j];
        }
        s_tilde.push_back((x == i ? 1 : 0)/(_k * _weights[i]) * j_sum);
    }
    _s_tilde_t.push_back(s_tilde);
    for (int i = 0; i < _k; i++) {
        double top = 0;
        double bottom = 0;
        for (int t = 0; t < round; t++) {
            top += _s_tilde_t[t][i];
        }
        for (int j = 0; j < _k; j++) {
            double bottom_inner = 0;
            for (int t = 0; t < round; t++){
                bottom_inner += _s_tilde_t[t][j];
            }
            bottom_inner *= _eta;
            bottom_inner = exp(bottom_inner);
            bottom += bottom_inner;
        }
        top = exp(_eta * top);
        double q_tilde = top/bottom;
        _weights[i] = (1 - _gamma) * q_tilde + (_gamma/_k);
    }
}

