

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3TOR_CPP
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3TOR_CPP

#include "Exp3Tor.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

Exp3Tor::Exp3Tor(int k, double eta)
        : _k(k), _eta(eta) {
    _probabilities = std::vector<double>();
    for (int i = 0; i < k; i++) {
        _probabilities.push_back(0);
    }
    _weights = std::vector<double>();
    _cnt = 0;
    // Init all to 1
    for (int i = 0; i < k; i++) {
        _weights.push_back(0);
    }
};

int Exp3Tor::sample() {
    std::discrete_distribution<int> d(_probabilities.begin(), _probabilities.end());
    int s = d(_random_gen);
    return s;
}

int Exp3Tor::choose() {
    double sum_reduced_power_weights = 0;
    double m = *max_element(_weights.begin(), _weights.end());
    for (int i = 0; i < _k; i++) {
        //std::cout << "val: "<< m << " " << _weights[i] << " " << i << " " << m << _eta << std::endl;
        sum_reduced_power_weights += exp((_eta) * (_weights[i]) - (_eta) * (m));
    }
    //std::cout << sum_reduced_power_weights << std::endl;

    for (int i = 0; i < _k; i++) {
        _probabilities[i] = exp((_eta) * (_weights[i]) - (_eta) * (m) - log(sum_reduced_power_weights));
        //std::cout << _probabilities[i] << " ";
    }
    //std::cout << std::endl;

    int choice = sample();

    return choice;
}

void Exp3Tor::give_reward(int index, double feedback) {
    _cnt++;
    for (int i = 0; i < _k; i++) {
        //int i = index;
        double est_loss = i == index ? (1 - feedback) / (_probabilities[i]) : 0;
        _weights[i] = _weights[i] + 1 - est_loss;
        if (std::isinf(_weights[i])) {
            std::cout << "INF!" << std::endl;
        }
    }
    if (_cnt%10000 == 0) {
        //std::cout << _weights[0] << " " << _weights[1] << " " << _weights[2] << " " << _weights[3] << " " << _weights[4]<< " " << _weights[5] << " " << _weights[6] << " " << _weights[7] << std::endl;
    }
    }


#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3TOR_CPP
