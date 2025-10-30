

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DEXP3_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DEXP3_H

#include "random"
#include "../../utilities/random_gen.h"
struct Dexp3_return_values {
    int choice1, choice2;
};
class DExp3 {
private:
    size_t _k;
    double _gamma;
    double _eta;
    std::mt19937 _random_gen = random_gen();
    std::vector<std::vector<double>> _s_tilde_t;
public:
    DExp3(size_t k, double gamma, double eta);
    Dexp3_return_values choose();
    void give_reward(size_t x, size_t y, size_t winner, int round);
    std::vector<double> _weights;
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_DEXP3_H
