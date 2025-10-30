

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_FPL_TOPLOG_H
#define EFFICIENT_MULTI_ARMED_BANDITS_FPL_TOPLOG_H
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"

class FPL_toplog
{
private:
    std::vector<double> _weights;
    std::mt19937 _gen;
    int _k;
    std::exponential_distribution<double> _exponential_distribution;
    int current_round;
    double _eta;
    int update_interval;
    std::vector<int> optimal_indices;
    std::vector<int> suboptimal_indices;

public:
    FPL_toplog(int k, double eta)
    {
        _weights = std::vector<double>(k, 1);
        _gen = random_gen();
        _exponential_distribution = std::exponential_distribution<double>(eta);
        _k = k;
        current_round = 0;
        _eta = eta;
        // update_interval = (int)k * log2(k);
        update_interval = k;
        current_round = 0;
    }

    void give_reward(size_t choice, double feedback)
    {
        _weights[choice] += feedback;
    }
    std::vector<double> &get_weights()
    {
        return _weights;
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
                v /= sum;
        }
        if (current_round % update_interval == 0)
        {
            auto indices = argsort(_weights);
            int number_optimal = (int)log2(_k);

            optimal_indices = std::vector<int>();
            for (int i = 0; i < number_optimal; i++)
            {
                optimal_indices.push_back(indices[i]);
            }
            suboptimal_indices = std::vector<int>();
            for (int i = number_optimal; i < _k; i++)
            {
                suboptimal_indices.push_back(indices[i]);
            }
        }

        for (int i = 0; i < optimal_indices.size(); i++)
        {
            _weights[optimal_indices[i]] += _exponential_distribution(_gen);
        }
        int offset = current_round % (int)((_k - log2(_k)) / log2(_k));

        for (int i = 0; i < (int)log2(_k); i++)
        {
            _weights[i * offset] += _exponential_distribution(_gen);
        }

        current_round++;

        if (current_round % (int)_eta == 0)
        {
            double sum = 0;
            for (auto v : _weights)
                sum += v;
            for (auto &v : _weights)
                v /= sum;
        }

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
#endif // EFFICIENT_MULTI_ARMED_BANDITS_FPL_TOPLOG_H