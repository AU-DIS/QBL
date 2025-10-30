


#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_HEAP_H
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
class Exp3m_heap
{
private:
    int _k;
    int _m;
    int cnt = 0;
    std::uniform_real_distribution<double> _uni;
    std::mt19937 _random_gen = random_gen();

public:
    std::vector<double> _weights;
    double _gamma;
    std::vector<double> _last_probabilities;
    Incremental_sum_heap _distribution;

    std::vector<double>* get_weights() {
        return &_weights;
    }


    Exp3m_heap() = default;

    Exp3m_heap(int m, int k, double gamma) : _m(m), _k(k), _gamma(gamma)
    {
        _weights = std::vector<double>(k, 1.0/_k);
        _distribution = Incremental_sum_heap(_weights);
        _last_probabilities.reserve(_k);
        //std::cout << _gamma << std::endl;
    };
    Exp3m_heap(int m, int k, int benchmode) : _m(m), _k(k)
    {
        _gamma = 0.1;
        _weights = std::vector<double>(k, 1.0/_k);
        _distribution = Incremental_sum_heap(_weights);
        _last_probabilities.reserve(_k);
        //std::cout << _gamma << std::endl;
    };
    std::vector<int> choose(int m)
    {
        auto choices = std::vector<int>(m, 0);
        auto priors = std::vector<double>(m, 0.);

        for (int i = 0; i < m; i++)
        {
            int a;
            if (_uni(_random_gen)<_gamma) {
                a = _distribution.heap_random_sample();
            } else {
                a = _distribution.heap_sample();
            }
            choices[i] = a;
            priors[i] = _distribution.probability_of_choice(a);
            //for (int i = 0; i < m; i++) {
            _last_probabilities[choices[i]] = (( (1 - _gamma) * exp(_weights[choices[i]] - _distribution.max_element()) + _gamma/(_k)));
            //}

            _distribution.update(a, std::numeric_limits<double>::infinity());
        }
        for (int i = 0; i < m; i++)
        {
            _distribution.update(choices[i], priors[i]);
        }

        //std::vector<int> choices = _distribution.heap_sample(m);




        return choices;
    };
    void give_reward(std::vector<int> &indices, std::vector<double> &rewards)
    {
        cnt++;
        //std::cout << indices.size() << std::endl;
        for (int idx = 0; idx < indices.size(); idx++) {
            int i = indices[idx];
            _weights[i] += _gamma * rewards[idx] / _last_probabilities[i]/_k; //implicit log
            if (cnt%10000 == 0) {
                //std::cout << _weights[0] << " " << _weights[1] << " "  << _weights[2] << " "  << _weights[3] << std::endl;
            }
            _distribution.update(i, _weights[i]);
        }

    };
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_HEAP_H
