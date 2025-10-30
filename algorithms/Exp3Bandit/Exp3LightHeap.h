

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3LIGHTHEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3LIGHTHEAP_H
#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"
#include "../../datastructures/Incremental_sum_heap.h"

class Exp3LightHeap {
private:
    int _k;
    double _eta;
    double _last_drawn_weight;
    double _last_drawn_probability;
    std::vector<double> _weights;
    std::vector<double> _probabilities;
    Incremental_sum_heap _distribution;
    std::mt19937 _random_gen = random_gen();
    //int sample();
public:
    Exp3LightHeap(int k, double eta);
    int choose();
    std::vector<double>* get_weights();
    void give_reward(int index, double feedback);
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3LIGHTHEAP_H
