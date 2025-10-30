

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_LIGHT_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_LIGHT_HEAP_H
#include <vector>
#include "cmath"
#include "algorithm"
#include "iostream"
#include "../../utilities/argsort.h"
#include "../../datastructures/Incremental_LSE_sum_heap.h"
#include "../../datastructures/Incremental_sum_heap.h"
#include "../../datastructures/Incremental_product_heap.h"
//#include "../utilities/random_gen.h"

#include "cmath"
class Exp3light_m_heap
{
private:
    int _k;
    int _m;
    int cnt = 0;
    std::uniform_real_distribution<double> _uni;
    std::mt19937 _random_gen = random_gen();

public:
    std::vector<double> _weights;
    double _eta;
    std::vector<double> _last_probabilities;
    Incremental_sum_heap _distribution;

    std::vector<double>* get_weights() {
        return &_weights;
    }


    Exp3light_m_heap() = default;

    Exp3light_m_heap(int m, int k, double eta) : _m(m), _k(k), _eta(eta)
    {
        _weights = std::vector<double>(k, 1);
        _distribution = Incremental_sum_heap(_weights);
        _last_probabilities.reserve(_k);
    };
    std::vector<int> choose(int m)
    {
        auto choices = std::vector<int>(m, 0);
        auto priors = std::vector<double>(m, 0.);
        auto z = std::vector<int>(m, 0);
        for (int i = 0; i < m; i++)
        {
            int a;
            //if (_uni(_random_gen)<0.1) {
            //    a = _distribution.heap_random_sample();
            //} else {
                a = _distribution.heap_sample();
            //}
            choices[i] = a;
            priors[i] = _distribution.probability_of_choice(a);
            z[i] = a;
            for (int i : choices) {
                //std::cout << i << " ";
                _last_probabilities[i] = exp(_weights[i] - _distribution.max_element());
            }
            //std::cout << std::endl;
            _distribution.update(a, std::numeric_limits<double>::infinity());
        }
        for (int i = 0; i < m; i++)
        {
            _distribution.update(choices[i], priors[i]);
        }
        //_distribution.print_internals();
        //_distribution.print_weights();



        return choices;
    };
    void give_reward(std::vector<int> &indices, std::vector<double> &rewards)
    {

        for (int idx = 0; idx < indices.size(); idx++) {
            int i = indices[idx];
            _weights[i] += -_eta*((1-rewards[idx]) / _last_probabilities[i]); //implicit log

            _distribution.update(i, _weights[i]);
        }

    };
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_LIGHT_HEAP_H

