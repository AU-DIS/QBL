

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3_CPP
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3_CPP

#include "Exp3.h"
#include <random>
#include <algorithm>
#include <iostream>

Exp3::Exp3(int k, double gamma)
    : _k(k), _gamma(gamma)
{
    _cnt = 0;
    _probabilities = std::vector<double>(k, 0.0);
    _weights = std::vector<double>(k, 1.0);
};

void Exp3::set_parameter(double gamma) {
    _gamma = gamma;
}

Exp3::Exp3(const Exp3 &prototype)
{


    _cnt = 0;
    _k = prototype._k;
    _gamma = prototype._gamma;
    _probabilities = std::vector<double>(prototype._k, 0.0);
    _weights = std::vector<double>(prototype._k, 1.0);
    //NOT THIS ONEASDAPÅSDIA`#"!=¤("?#(?#"(%"?"(%#(?"(

    _random_gen = random_gen();
}

int Exp3::sample()
{
    std::discrete_distribution<int> d(_probabilities.begin(), _probabilities.end());
    int s = d(_random_gen);
    return s;
}

int Exp3::choose()
{
    double _eta = 1;
    double sum_reduced_power_weights = 0;
    double m = *max_element(_weights.begin(), _weights.end());

    for (int i = 0; i < _k; i++)
    {
        sum_reduced_power_weights += exp( _weights[i] - m);
    }
    if (_cnt%10000 == 0) {
        //std::cout << sum_reduced_power_weights << " " << m << std::endl;
        //std::cout << _weights[0] << " " << _weights[1] << " " << _weights[2] << " " << _weights[3] << std::endl;
    }

    for (int i = 0; i < _k; i++)
    {
        _probabilities[i] = (1 - _gamma) * exp(_weights[i] - m - log(sum_reduced_power_weights)) + _gamma / _k;

    }

    int choice = sample();
    _last_drawn_probability = _probabilities[choice];
    _last_drawn_weight = _weights[choice];

    return choice;
}

void Exp3::give_reward(int index, double feedback)
{

    _cnt++;
    double est_reward = _gamma*feedback / _last_drawn_probability /_k;
    double new_weight = _last_drawn_weight + est_reward;
    _weights[index] = new_weight;
    if (_cnt%10000 == 0) {
        //std::cout << _weights[0] << " " << _weights[1] << " " << _weights[2] << " " << _weights[3] << " " << _weights[4]<< " " << _weights[5] << " " << _weights[6] << " " << _weights[7] << std::endl;
    }

}

    std::vector<double> *Exp3::get_weights()
    {
        return &_weights;
    }

#endif // EFFICIENT_MULTI_ARMED_BANDITS_EXP3_CPP
