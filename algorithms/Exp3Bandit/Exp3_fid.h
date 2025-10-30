

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3_DEFERRED_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3_DEFERRED_H


#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"

class Exp3_fid {
private:
    double _last_drawn_weight;
    double _last_drawn_probability;
    int rounds_distribution_used;
    std::vector<double> _probabilities;
    std::mt19937 _random_gen;
    int sample() {
        if (rounds_distribution_used % _xi == 0) {
            double sum_reduced_power_weights = 0;
            double m = *max_element(_weights.begin(), _weights.end());
            for (int i = 0; i < _k; i++) {
                sum_reduced_power_weights += exp(_gamma / _k * (_weights[i] - m));
            }

            for (int i = 0; i < _k; i++) {
                _probabilities[i] = (1 - _gamma) * exp(_gamma / _k * (_weights[i] - m) - log(sum_reduced_power_weights)) + _gamma / _k;
            }
            d = std::discrete_distribution<int>(_probabilities.begin(), _probabilities.end());
        }
        rounds_distribution_used++;
        int s = d(_random_gen);
        return s;
    }
    std::discrete_distribution<int> d;

public:
    std::vector<double> _weights;
    int _k;
    double _gamma;
    int _xi;

    Exp3_fid(int k, double gamma, int xi=2) {
        _k = k;
        _gamma = gamma;
        _xi = xi;

        rounds_distribution_used = 0;
        _probabilities = std::vector<double>(k, 0);
        _weights = std::vector<double>(k, 1.0);

        _random_gen = random_gen();

        choose();
    }

    Exp3_fid(const Exp3_fid &prototype) {
        _k = prototype._k;
        _gamma = prototype._gamma;
        _xi = prototype._xi;

        rounds_distribution_used = 0;
        _probabilities = std::vector<double>(_k, 0);
        _weights = std::vector<double>(_k, 1.0);

        _random_gen = random_gen();

        choose();
    }
    int choose() {
        int choice = sample();
        _last_drawn_probability = _probabilities[choice];
        _last_drawn_weight = _weights[choice];

        return choice;

    }
    void give_reward(int index, double feedback) {
        double est_reward = feedback / _last_drawn_probability;
        double new_weight = _last_drawn_weight + est_reward;
        _weights[index] = new_weight;
    }
    std::vector<double> *get_weights()
    {
        return &_weights;
    }

};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3_DEFERRED_H
