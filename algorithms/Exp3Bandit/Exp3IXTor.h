

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3IXTOR_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3IXTOR_H
#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"

class Exp3IXTor {
private:
    int _k;
    double _eta;
    double _gamma;
    double _last_drawn_weight;
    double _last_drawn_probability;
    std::vector<double> _weights;
    std::vector<double> _probabilities;
    std::mt19937 _random_gen = random_gen();
    int sample();
public:
    Exp3IXTor(int k, double eta, double gamma);
    Exp3IXTor(int k, double eta);
    int choose();
    void give_reward(int index, double feedback);
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3IXTOR_H
