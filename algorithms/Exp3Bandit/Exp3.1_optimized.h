

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP31_optimized_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP31_optimized_H
#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"
#include "Exp3.h"

class Exp31_optimized
{
private:
    int _k;
    int _r;
    double _gamma;
    int round;
    double max_acc;
    double max_index;

    std::vector<double> accumulated_rewards;
    Exp3 _exp3;

public:
    explicit Exp31_optimized(int k)
    {
        _k = k;
        accumulated_rewards = std::vector<double>(k, 0);
        _gamma = 1.0;
        _exp3 = Exp3(_k, _gamma);
        _r = 0;
        round = 0;
        max_acc = 0;
        max_index = 0;
    }
        std::vector<double> *get_weights()
    {
        return _exp3.get_weights();
    }
    int choose()
    {
        double f_term = _k * log(_k);
        double s_term = exp(1.0) - 1.0;
        double _g_r = (f_term / s_term) * pow(4, _r);

        if (max_acc > (_g_r - _k / _gamma))
        {
            _r += 1;
            _gamma /= 2.0;
            _exp3 = Exp3(_k, _gamma);
        }
        return _exp3.choose();
    }
    void give_reward(int index, double feedback)
    {
        accumulated_rewards[index] += feedback;
        if (accumulated_rewards[index] > max_acc)
        {
            max_acc = accumulated_rewards[index];
            max_index = index;
        }
        _exp3.give_reward(index, feedback);
    }
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP31_optimized_H