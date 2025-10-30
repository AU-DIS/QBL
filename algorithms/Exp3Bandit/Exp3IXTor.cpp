
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3IXTOR_CPP
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3IXTOR_CPP

#include "Exp3IXTor.h"
#include <random>
#include <algorithm>
#include <iostream>

Exp3IXTor::Exp3IXTor(int k, double eta, double gamma)
        : _k(k), _eta(eta), _gamma(gamma) {
    _probabilities = std::vector<double>();
    for (int i = 0; i < k; i++) {
        _probabilities.push_back(0);
    }
    _weights = std::vector<double>();
    // Init all to 0
    for (int i = 0; i < k; i++) {
        _weights.push_back(0);
    }
};
Exp3IXTor::Exp3IXTor(int k, double eta)
        : _k(k), _eta(eta) {
    _gamma = 2*_eta;
    _probabilities = std::vector<double>();
    for (int i = 0; i < k; i++) {
        _probabilities.push_back(0);
    }
    _weights = std::vector<double>();
    // Init all to 0
    for (int i = 0; i < k; i++) {
        _weights.push_back(0);
    }
};

int Exp3IXTor::sample() {
    std::discrete_distribution<int> d(_probabilities.begin(), _probabilities.end());
    int s = d(_random_gen);
    return s;
}

int Exp3IXTor::choose() {
    double sum_reduced_power_weights = 0;
    double m = *max_element(_weights.begin(), _weights.end());
    for (int i = 0; i < _k; i++) {
        sum_reduced_power_weights += exp(-_eta * _weights[i] - (-_eta * m));
    }

    for (int i = 0; i < _k; i++) {
        _probabilities[i] = exp(-_eta * _weights[i] - (-_eta * m) - log(sum_reduced_power_weights));
    }

    int choice = sample();
    //_last_drawn_probability = _probabilities[choice];
    //_last_drawn_weight = _weights[choice];

    return choice;
}

void Exp3IXTor::give_reward(int index, double feedback) {
    double est_loss = (1 - feedback) / (_probabilities[index] + _gamma);
    _weights[index] = _weights[index] + est_loss;
    if (std::isinf(_weights[index])) {
        std::cout << "INF!" << std::endl;
    }
}


#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3IXTOR_CPP
