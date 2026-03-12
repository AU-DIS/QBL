

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_FPL_HASH_H
#define EFFICIENT_MULTI_ARMED_BANDITS_FPL_HASH_H

#include "../../utilities/random_gen.h"
class FPL_hash
{
private:
    std::vector<double> _weights;
    std::exponential_distribution<double> _exponential_distribution;
    std::mt19937 _gen;
    std::vector<double> _presampled_noise;
    double _a;
    double _b;
    long mersenne_8 = 2147483647;
    int current_round;
    unsigned int hash(double w)
    {
        auto r = (((unsigned int)(_a * w + _b)) % mersenne_8) % _number_to_presample;
        return (int)r;
    }

public:
    int _k;
    double _eta;
    int _number_to_presample;

    FPL_hash(int k, double eta, int rounds)
    {
        _k = k;
        _eta = eta;
        _number_to_presample = rounds;
        _exponential_distribution = std::exponential_distribution<double>(_eta);
        _gen = random_gen();
        _weights = std::vector<double>(k, 0);
        for (auto &w : _weights)
            w += _exponential_distribution(_gen);

        _presampled_noise = std::vector<double>(_number_to_presample, 0);

        for (int i = 0; i < _number_to_presample; i++)
        {
            _presampled_noise[i] = (_exponential_distribution(_gen));
        }
        // for (auto w : *_presampled_noise) std::cout << w << ",";

        std::uniform_real_distribution<double> uni(0.0, 1.0);

        _a = uni(_gen) * _number_to_presample;
        _b = uni(_gen) * _number_to_presample;

        current_round = 0;
    }

    FPL_hash(FPL_hash &prototype)
    {
        _k = prototype._k;
        _eta = prototype._k;
        _number_to_presample = prototype._number_to_presample;
        _exponential_distribution = std::exponential_distribution<double>(_eta);
        _gen = random_gen();
        _weights = std::vector<double>(_k, 0);
        for (auto &w : _weights)
            w += _exponential_distribution(_gen);

        _presampled_noise = std::vector<double>(_number_to_presample, 0);

        for (int i = 0; i < _number_to_presample; i++)
        {
            _presampled_noise[i] = (_exponential_distribution(_gen));
        }
        // for (auto w : *_presampled_noise) std::cout << w << ",";

        std::uniform_real_distribution<double> uni(0.0, 1.0);

        _a = uni(_gen) * _number_to_presample;
        _b = uni(_gen) * _number_to_presample;

        current_round = 0;
    }

    int choose()
    {
        current_round++;
        if (current_round % (int)_eta == 0)
        {
            double sum = 0;
            for (auto v : _weights)
                sum += v;
            for (auto &v : _weights)
                v = v / sum;
        }

        for (auto &w : _weights)
        {
            auto h = hash(w);
            int index = h % _number_to_presample;
            w += _presampled_noise[index];
            // std::cout << (*_presampled_noise)[index] << ",";
        }
        int max_index = 0;
        double max_element = 0;

        for (int i = 0; i < _k; i++)
        {
            if (_weights[i] > max_element)
            {
                max_index = i;
                max_element = _weights[i];
            }
        }
        return max_index;
    }
    void give_reward(int choice, double feedback)
    {
        _weights[choice] += feedback;
    }
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_FPL_HASH_H
