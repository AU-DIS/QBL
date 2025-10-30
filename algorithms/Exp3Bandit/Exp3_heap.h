

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3_HEAP_H
#include "../../datastructures/Incremental_sum_heap.h"

class Exp3_heap
{

public:
    Incremental_sum_heap _distribution;
    int _k;
    double _gamma;

    Exp3_heap(int k, double gamma)
    {
        _k = k;
        _gamma = gamma;
        _weights = std::vector<double>(k, 1.0);
        _distribution = Incremental_sum_heap(_weights);
        _additive_term = _gamma / _k;

        //std::vector<double> power_weights(k, exp(_gamma / _k * ((1.0 / k)) - (1.0 / k)));
        //_power_weights = Incremental_sum_heap(power_weights);

        max_weight = 1.0;
    }


    Exp3_heap(const Exp3_heap &prototype)
    {
        /*_k = prototype._k;
        _gamma = prototype._gamma;
        _weights = std::vector<double>(_k, 1.0 / _k);
        _distribution = Incremental_sum_heap(_weights);
        _additive_term = _gamma / _k;

        std::vector<double> power_weights(_k, exp(_gamma / _k * ((1.0 / _k)) - (1.0 / _k)));
        _power_weights = Incremental_sum_heap(power_weights);

        max_weight = 1.0 / _k;*/
    }

    int choose()
    {
        int v;
        if (_uni(_random_gen)<_gamma) {
            v = rand()%_k;
        } else {
            v = _distribution.heap_sample();
        }
        return v; //_distribution.heap_sample();
    }
    std::tuple<int, bool> choose(double gamma)
    {
        auto [index, gamma_obs] = _distribution.heap_sample_observe(gamma);
        return {index, gamma_obs};
    }
    std::vector<double> *get_weights()
    {
        return &_weights;
    }

    void give_reward(int index, double feedback)
    {
        //double probability = (1 - _gamma) * exp(_gamma / _k * (_weights[index] - max_weight) - log(_distribution.max_element())) + _gamma / _k;
        double probability = (1 - _gamma) * exp(_distribution.probability_of_choice(index) - _distribution.max_element()) + _gamma / _k;

        double est_reward = feedback / probability;
        //_weights[index] += est_reward;

        _distribution.update(index, _distribution.probability_of_choice(index)+_gamma*(est_reward/_k));
        //_power_weights.update(index, exp(_gamma / _k * (_weights[index] - max_weight)));
        //if (_weights[index] > max_weight)
        //    max_weight = _weights[index];
    }

private:
    double max_weight;
    std::vector<double> _weights;
    double _additive_term;
    //Incremental_sum_heap _power_weights;
    std::uniform_real_distribution<double> _uni;
    std::mt19937 _random_gen = random_gen();
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP3_HEAP_H
