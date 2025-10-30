

#include "vector"
#include "random"
#include "../../utilities/random_gen.h"

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DEBUG_SIMPLE_ADVERSARIAL_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DEBUG_SIMPLE_ADVERSARIAL_H


class debug_simple_adversarial {
public:
    debug_simple_adversarial(int K, int rounds);
    double feedback(int choice, double &regret);
    int k = 0;
    double max_regret = 0;
private:
    std::vector<double> _data_matrix;
    void randomize_weights();
    std::mt19937 _gen = random_gen();
    std::normal_distribution<double> _d;
    int _rounds = 0;
    int _current_round = 0;
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_DEBUG_SIMPLE_ADVERSARIAL_H
