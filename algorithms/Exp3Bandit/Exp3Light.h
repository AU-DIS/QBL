

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3LIGHT_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3LIGHT_H
#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"

class Exp3Light {
private:
    int _k;
    double _eta;
    double _SE = 0;
    double _max;
    int _cnt;
    double _last_drawn_weight;
    double _last_drawn_probability;
    std::vector<double> _weights;
    std::vector<double> _probabilities;
    std::mt19937 _random_gen = random_gen();
    int sample();
public:
    Exp3Light(int k, double eta);
    int choose();
    std::vector<double>* get_weights();
    void give_reward(int index, double feedback);
};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3LIGHT_H
