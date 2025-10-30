

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_H

#include <vector>
#include "cmath"
#include "algorithm"
#include "iostream"
#include "../../utilities/argsort.h"
#include "DepRoundALIASStrategy.h"

#include "cmath"

class Exp3m {
private:
    int _k;
    int _m;
    DepRoundALIASStrategy _sampling;
    double get_alpha(double rhs, std::vector<size_t> &argsorted) {
        double weight_sum = 0;
        for (auto v : _weights) weight_sum += v;
        for (size_t i = 0; i < _k; i++) {
            double alpha_candidate = (log(rhs) + weight_sum) - log((1 - i * rhs));
            double current_value = _weights[argsorted[_k-1-i]];
            if (alpha_candidate > current_value) {
                return alpha_candidate;}
            weight_sum += log(1-exp(current_value-weight_sum));
        }
        //return (log(rhs)) - log((1 - _k * rhs));
        abort();
    };
public:
    double _gamma;
    std::vector<double> _last_probabilities;
    std::vector<double> _weights;
    std::vector<double> weights_prime;

    std::vector<double>* get_weights() {
        return &_weights;
    }

    Exp3m() = default;
    Exp3m(int m, int k, double gamma) : _m(m), _k(k), _gamma(gamma) {
        _sampling = DepRoundALIASStrategy();
        weights_prime = std::vector<double>(k, 1.0/_k);
        for (int i = 0; i < _k; i++) {
            _weights.push_back(1.0/_k);
        }
    };
    Exp3m(int m, int k, int benchmode) : _m(m), _k(k) {
        _gamma = 0.1;
        _sampling = DepRoundALIASStrategy();
        weights_prime = std::vector<double>(k, 1.0/_k);
        for (int i = 0; i < _k; i++) {
            _weights.push_back(1.0/_k);
        }
    };
    std::vector<int> choose(int m) {
        /*auto sorted_weight_indices = argsort(_weights);

        double sum_weights = 0;
        for (double v: _weights) {
            if (sum_weights > v) {
                sum_weights += log(exp(v-sum_weights)+1);
            } else {
                sum_weights = v + log(exp(sum_weights-v)+1);
            }
        }

        double threshold = log((1.0/_m-_gamma/_k))+sum_weights-log((1-_gamma)); //in log scale
        //std::vector<double> weights_prime;
        //std::cout << "back" << sorted_weight_indices.back() << std::endl;
        //std::cout << "front" << sorted_weight_indices.front() << std::endl;
        if (_weights[sorted_weight_indices.back()] >= threshold) {
            //Decide alpha
            double rhs =  (1.0/_m-_gamma/_k)/(1-_gamma);
            double alpha_t = get_alpha(rhs, sorted_weight_indices);
            for (size_t i = 0; i < _k; i++) {
                weights_prime[i] = (std::min(alpha_t, _weights[i]));
            }
        }
        else {
            for (size_t i = 0; i < _k; i++) {
                weights_prime[i] = _weights[i];
            }
        }
        double w_sum = 0;
        for (auto v: weights_prime) {
            if (w_sum > v) {
                w_sum += log(exp(v-w_sum)+1);
            } else {
                w_sum = v + log(exp(w_sum-v)+1);
            }
        }*/

        std::vector<double> probabilities;
        probabilities.reserve(_k);

        double sum_exp = 0;
        double mw = *max_element(_weights.begin(), _weights.end());
        for (int i = 0; i < _k; i++)
        {
            sum_exp += exp(_weights[i] - mw);
        }


        for (int i = 0; i < _k; i++) {

            double p = _m * ( (1-_gamma)*exp(_weights[i]- mw - log(sum_exp)) + _gamma/_k);
            //std::cout << w_sum << p << std::endl;
            probabilities.push_back(p);
        }
        _last_probabilities = probabilities;
        return _sampling.dependent_weighted_choices(probabilities, _m);
    };
    void give_reward(std::vector<int> &indices, std::vector<double> &rewards) {
        for (int i = 0; i < indices.size(); i++) {
            _weights[indices[i]] +=  _m*_gamma*(rewards[i]/_last_probabilities[indices[i]])/_k;
        }
        /*double sum_weights = 0;
        for (double v: _weights) sum_weights += v;
        for (double & _weight : _weights) {
            _weight = _weight/sum_weights;
        }*/

    };
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3M_H
