


#include "Exp3Light.h"
#include <algorithm>
#include <random>
#include <iostream>

Exp3Light::Exp3Light(int k, double eta)
        : _k(k), _eta(eta)
{
    _cnt = 0;
    _probabilities = std::vector<double>(k, 0.0);
    _weights = std::vector<double>(k, 1.0);
    _max = 1;
};

int Exp3Light::sample()
{
    std::discrete_distribution<int> d(_probabilities.begin(), _probabilities.end());
    int s = d(_random_gen);
    return s;
}

int Exp3Light::choose()
{
    double sum_reduced_power_weights = 0;
    double m = *max_element(_weights.begin(), _weights.end());

    for (int i = 0; i < _k; i++)
    {
        sum_reduced_power_weights += exp(_weights[i] - m);
    }
    if (_cnt%10000 == 0) {
        //std::cout << sum_reduced_power_weights << " " << m << std::endl;
        //std::cout << _weights[0] << " " << _weights[1] << " " << _weights[2] << " " << _weights[3] << std::endl;
    }

    for (int i = 0; i < _k; i++)
    {
        //_probabilities[i] = (1 - _gamma) * exp(_eta*_weights[i] - _eta*m - log(sum_reduced_power_weights)) + _gamma / _k;
        _probabilities[i] = exp(_weights[i] - m - log(sum_reduced_power_weights));
    }

    int choice = sample();
    _last_drawn_probability = _probabilities[choice];
    _last_drawn_weight = _weights[choice];

    return choice;
}

void Exp3Light::give_reward(int index, double feedback)
{
    int i = index;
    _cnt++;
    //double est_reward = _gamma*feedback / _last_drawn_probability /_k;
    double est_reward = -_eta*(1-feedback) / _last_drawn_probability;
    double new_weight = _last_drawn_weight + (est_reward);
    _weights[index] = new_weight;
    if (_cnt%10000 == 0) {
        //std::cout << _weights[0] << " " << _weights[1] << " " << _weights[2] << " " << _weights[3] << " " << _weights[4]<< " " << _weights[5] << " " << _weights[6] << " " << _weights[7] << std::endl;
    }
    double _w_old = new_weight;
    //_weights[i] += -_eta*((1-rewards[idx]) / _last_probabilities[i]); //implicit log
    //_distribution.update(i, _weights[i]);


}

std::vector<double> *Exp3Light::get_weights()
{
    return &_weights;
}



