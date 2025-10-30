

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP31_CPP
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP31_CPP

#include <cmath>
#include "vector"
#include <random>
#include <algorithm>
#include "Exp31.h"
#include <iostream>

Exp31::Exp31(int k) : _k(k)
{
    accumulated_rewards = std::vector<double>(k, 0);
    _g_r = (_k * log(_k)) / (exp(1) - 1) * pow(4, _r);
    _gamma = std::min(1.0, sqrt(_k * log(_k) / ((exp(1) - 1) * _g_r)));
    _exp3 = Exp3(_k, _gamma);
    _r = 0;
    round = 0;
}

int Exp31::choose()
{
    double f_term = _k * log(_k);
    double s_term = exp(1.0) - 1.0;

    auto g_max = *std::max_element(accumulated_rewards.begin(), accumulated_rewards.end());
    if (g_max > (_g_r - _k / _gamma))
    {
        _r += 1;
        _g_r = (f_term / s_term) * pow(4, _r);
        _gamma = std::min(1.0, sqrt(_k * log(_k) / ((exp(1) - 1) * _g_r)));
        _exp3 = Exp3(_k, _gamma);
    }
    round += 1;
    return _exp3.choose();
}

void Exp31::give_reward(int index, double feedback)
{
    accumulated_rewards[index] += feedback;
    _exp3.give_reward(index, feedback);
}

std::vector<double> *Exp31::get_weights()
    {
        return _exp3.get_weights();
    }

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP31_CPP