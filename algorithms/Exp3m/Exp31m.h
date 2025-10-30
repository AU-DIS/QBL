

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP31M_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP31M_H

#include <vector>
#include "algorithm"
#include "iostream"
#include "../../utilities/argsort.h"
#include "Exp3m.h"


class Exp31m {
private:
    int _k;
    int _m;
    int _r;
    double _g_r;
    double _gamma;
    int round;

    std::vector<double> accumulated_rewards;
    std::vector<double> old_w;
    Exp3m _exp3m;

public:
    Exp31m(int m, int k, Exp3m &b) : _k(k), _m(m), _exp3m(b) {
        accumulated_rewards = std::vector<double>(k, 0);
        _g_r = (_k * log((double)_k/(double)_m)) / (exp(1) - 1) * _m * pow(4, _r);
        _gamma = std::min(1.0, sqrt(_k * log((double)_k/(double)_m) / ((exp(1) - 1)* _m * _g_r)));
        _exp3m = Exp3m(_m, _k, _gamma);
        _r = 0;
        round = 0;

        for (int i = 0; i < _k; i++) accumulated_rewards.push_back(0);
    };

    std::vector<int> choose(int m) {

        auto g_max = *std::max_element(accumulated_rewards.begin(), accumulated_rewards.end());
        if (g_max > (_g_r - _k / _gamma))
        {
            _r += 1;
            _g_r = (_k * log((double)_k/(double)_m)) / (exp(1) - 1) * _m * pow(4, _r);
            _gamma = std::min(1.0, sqrt(_k * log((double)_k/(double)_m) / ((exp(1) - 1)* _m * _g_r)));
            //old_w = std::vector<double>(_exp3m._weights.begin(), _exp3m._weights.end());
            _exp3m = Exp3m(_m, _k, _gamma);
            //_exp3m._weights = old_w;
        }
        round += 1;
        return _exp3m.choose(m);
    };

    void give_reward(std::vector<int> &indices, std::vector<double> &rewards) {
        for (int i = 0; i < indices.size(); i++ ) {
            int index = indices[i];
            accumulated_rewards[index] += rewards[i]/_exp3m._last_probabilities[index];
        }
        _exp3m.give_reward(indices, rewards);
    }


};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP31M_H
