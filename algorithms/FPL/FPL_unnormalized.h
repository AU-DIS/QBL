

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_FPL_unnormalized_H
#define EFFICIENT_MULTI_ARMED_BANDITS_FPL_unnormalized_H
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"
#include "iostream"

class FPL_unnormalized
{
private:
    std::vector<double> _weights;
    std::mt19937 _gen;
    int _k;
    std::exponential_distribution<double> _exponential_distribution;
    int current_round;
    double _eta;

public:
    FPL_unnormalized() = default;
    FPL_unnormalized(int k, double eta)
    {
        _weights = std::vector<double>(k, 1);
        _gen = random_gen();
        _exponential_distribution = std::exponential_distribution<double>(eta);
        _k = k;
        current_round = 0;
        _eta = eta;
    }
    FPL_unnormalized(FPL_unnormalized &b)
    {
        _weights = std::vector<double>(b._k, 1);
        _gen = random_gen();
        _exponential_distribution = std::exponential_distribution<double>(b._eta);
        _k = b._k;
        current_round = 0;
        _eta = b._eta;
    }

    void set_parameter(double eta)
    {
        _eta = eta;
        _exponential_distribution = std::exponential_distribution<double>(eta);
    }

    void give_reward(size_t choice, double feedback)
    {
        _weights[choice] += feedback;
    }
    std::vector<double>* get_weights()
    {
        return &_weights;
    }

    int choose()
    {

        current_round++;
        for (auto &v : _weights)
            v += _exponential_distribution(_gen);
        /*
         * Draw is simply an arg-max, but std::vector does not support argmax
         * All code examples use chaining of algorithms that will only add useless computations,
         * so we implement it manually
         */
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
    std::vector<int> choose(int K)
    {
        return std::vector<int>(_k, 1);
    }
    void give_reward(int choice, double feedback)
    {
        _weights[choice] += feedback;
    }

    void give_reward(std::vector<int> &indices, std::vector<double> &rewards)
    {
    }
};
#endif // EFFICIENT_MULTI_ARMED_BANDITS_FPL_unnormalized_H