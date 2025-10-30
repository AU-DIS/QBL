

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3_H
#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"

class Exp3
{
private:
    double _last_drawn_weight;
    double _last_drawn_probability;
    std::vector<double> _probabilities;
    std::mt19937 _random_gen = random_gen();
    int sample();

public:
    Exp3() = default;
    Exp3(int k, double gamma);
    Exp3(const Exp3 &prototype);
    int choose();
    void give_reward(int index, double feedback);
    void set_parameter(double gamma);
    std::vector<double>* get_weights();
    std::vector<double> _weights;
    int _k;
    double _gamma;
    int _cnt;
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP3_H
