
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3_AVERAGE_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3_AVERAGE_H

#include <math.h>
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"
#include <queue>

class Exp3_lad
{
private:
    double _last_drawn_weight;
    double _last_drawn_probability;
    double _running_avg;
    int _round_counter;
    int _no_avg;
    std::vector<double> _running_observations;

    std::vector<double> _probabilities;
    std::mt19937 _random_gen;

    int sample()
    {
        recompute_average();

        if (_running_avg >= _avg_threshold)
        {
            double sum_reduced_power_weights = 0;
            double m = *max_element(_weights.begin(), _weights.end());
            for (int i = 0; i < _k; i++)
            {
                sum_reduced_power_weights += exp(_gamma / _k * (_weights[i] - m));
            }
            for (int i = 0; i < _k; i++)
            {
                _probabilities[i] = (1 - _gamma) * exp(_gamma / _k * (_weights[i] - m) - log(sum_reduced_power_weights)) + _gamma / _k;
            }
            d = std::discrete_distribution<int>(_probabilities.begin(), _probabilities.end());
        }
        int s = d(_random_gen);
        return s;
    }

    void recompute_average()
    {
        for (auto v : _running_observations)
            _running_avg += v;
        _running_avg /= _no_avg;
        // std::cout << _running_avg << std::endl;
    }

    std::discrete_distribution<int> d;

public:
    std::vector<double> _weights;
    int _k;
    double _gamma;
    double _avg_threshold;

    Exp3_lad(int k, double gamma, double avg_threshold)
    {
        _k = k;
        _gamma = gamma;
        _running_avg = 0;
        _avg_threshold = avg_threshold;
        _round_counter = 0;

        _no_avg = 5;
        _running_observations = std::vector<double>(_no_avg, 1);

        recompute_average();

        _probabilities = std::vector<double>(k, 0);
        _weights = std::vector<double>(k, 1.0);

        _random_gen = random_gen();

        choose();
    }

    Exp3_lad(const Exp3_lad &prototype)
    {
        _k = prototype._k;
        _gamma = prototype._gamma;
        _avg_threshold = prototype._avg_threshold;
        _running_avg = 0;
        _round_counter = 0;

        _no_avg = 5;
        _running_observations = std::vector<double>(_no_avg, 1);

        recompute_average();

        _probabilities = std::vector<double>(_k, 0);
        _weights = std::vector<double>(_k, 1.0);

        _random_gen = random_gen();

        choose();
    }

    int choose()
    {
        int choice = sample();
        _last_drawn_probability = _probabilities[choice];
        _last_drawn_weight = _weights[choice];

        return choice;
    }
    void give_reward(int index, double feedback)
    {
        double est_reward = feedback / _last_drawn_probability;
        double new_weight = _last_drawn_weight + est_reward;
        _weights[index] = new_weight;
        _running_observations[_round_counter % _no_avg] = 1 - feedback;
        _round_counter++;
    }
    std::vector<double> *get_weights()
    {
        return &_weights;
    }
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP3_AVERAGE_H
