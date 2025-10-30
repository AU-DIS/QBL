
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXP3R_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXP3R_H

#include <vector>
#include <tuple>
#include "Exp3_heap.h"

class Exp3r
{
private:
    double max_weight;
    std::vector<double> _weights;
    Exp3_heap _exp3 = Exp3_heap(4, 0.1);
    double _additive_term;
    bool last_was_gamma_obs;
    //Incremental_sum_heap _power_weights;

public:
    int _k;
    int _H;
    double _gamma;
    double _delta;
    std::vector<double> _arm_cum_gamma_obs_reward;
    std::vector<double> _arm_cum_gamma_obs_choices;
    long long round = 0;


    Exp3r(int k, double gamma, double delta, int H)
    {
        _k = k;
        _H = H;
        _gamma = gamma;
        _delta = delta;
        _weights = std::vector<double>(k, 1.0);
        //max_weight = 1.0 / k;
        _exp3 = Exp3_heap(k, gamma);

        _arm_cum_gamma_obs_reward = std::vector<double>(_k, 0);
        _arm_cum_gamma_obs_choices = std::vector<double>(_k, 0);
    }



    /*Exp3r(const Exp3_heap &prototype)
    {
        _k = prototype._k;
        _gamma = prototype._gamma;
        _weights = std::vector<double>(_k, 1.0 / _k);
        _distribution = Incremental_sum_heap(_weights);
        _additive_term = _gamma / _k;

        std::vector<double> power_weights(_k, exp(_gamma / _k * ((1.0 / _k)) - (1.0 / _k)));
        _power_weights = Incremental_sum_heap(power_weights);

        max_weight = 1.0 / _k;
    }*/

    int choose()
    {
        auto [index, gamma_obs] = _exp3.choose(_gamma);
        last_was_gamma_obs = gamma_obs;
        return index;
    }
    std::vector<double> *get_weights()
    {
        return _exp3.get_weights();
    }

    void give_reward(int index, double feedback)
    {
        _exp3.give_reward(index, feedback);
        round++;
        if (last_was_gamma_obs) {
            _arm_cum_gamma_obs_reward[index] += feedback;
            _arm_cum_gamma_obs_choices[index] += 1;
        }


        //TODO: Keep track of this to avoid scan
        double big_gamma = *std::min_element(_arm_cum_gamma_obs_reward.begin(), _arm_cum_gamma_obs_reward.end());

        if (big_gamma >= _gamma*_H/_k) {
            if (drift_detect()) {
                _exp3 = Exp3_heap(_k, _gamma);
            }
            _arm_cum_gamma_obs_reward = std::vector<double>(_k, 0);
            _arm_cum_gamma_obs_choices = std::vector<double>(_k, 0);
        }
    }

    bool drift_detect() {
        std::vector<double> *weights = _exp3.get_weights();

        auto result = std::max_element((*weights).begin(), (*weights).end());

        int k_max = std::distance((*weights).begin(), result);
        double epsilon = sqrt((_k*log(1/_delta))/(2.0*_gamma*_H));
        for (int i = 0; i < _k; i++) {
            double mean = _arm_cum_gamma_obs_reward[i]/_arm_cum_gamma_obs_choices[i];
            double mean_max = _arm_cum_gamma_obs_reward[k_max]/_arm_cum_gamma_obs_choices[k_max];
            if (mean-mean_max >= 2*epsilon)

                return true;
        }
        return false;
    }



};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXP3R_H
