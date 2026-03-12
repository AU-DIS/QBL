#ifndef EFFICIENT_MULTI_ARMED_BANDITS_FPL_BUCKETS_H
#define EFFICIENT_MULTI_ARMED_BANDITS_FPL_BUCKETS_H
#include <vector>
#include <random>
#include "../../utilities/random_gen.h"
#include "../../utilities/updateable_priority_queue.h"

class FPL_buckets
{
private:
    std::mt19937 _gen;
    int _k;
    double _eta;
    std::vector<int> upper_indices;
    std::vector<int> lower_indices;
    std::vector<int> index_to_queue;
    std::exponential_distribution<double> _exponential_distribution;
    better_priority_queue::updatable_priority_queue<int, double> U_max;
    better_priority_queue::updatable_priority_queue<int, double> U_min;
    better_priority_queue::updatable_priority_queue<int, double> L_max;
    better_priority_queue::updatable_priority_queue<int, double> L_min;
    int split_line;
    int upper_update_interval;
    int lower_update_interval;
    int current_round;

public:
    FPL_buckets(int k, double eta)
    {
        _gen = random_gen();
        _exponential_distribution = std::exponential_distribution<double>(eta);
        _k = k;
        _eta = eta;
        current_round = 0;
        // Super fast round up int division
        // split_line = k / 2 + (k % 2 != 0);
        // split_line = k - 6;
        int I_1 = (int)(k * log2(2)) / log2(k);
        split_line = k - I_1;
        // std::cout << "k = " + std::to_string(k) + " split_line = " + std::to_string(split_line) << std::endl;

        // lower_update_interval = split_line * (int)log2(k);
        lower_update_interval = split_line;
        upper_update_interval = 1;

        // The first half start in the lowers
        for (int i = 0; i < split_line; i++)
        {
            lower_indices.push_back(i);
            index_to_queue.push_back(0);
            L_max.push(i, 1);
            L_min.push(i, -1);
        }
        for (int i = split_line; i < k; i++)
        {
            U_max.push(i, 1);
            U_min.push(i, -1);
            upper_indices.push_back(i);
            index_to_queue.push_back(1);
        }
        current_round++;
    }

    void give_reward(size_t choice, double feedback)
    {
        if (index_to_queue[choice] == 0)
        {
            L_max.update(choice, L_max.get_priority(choice).second + feedback);
            L_min.update(choice, L_min.get_priority(choice).second - feedback);
        }
        else
        {
            U_max.update(choice, U_max.get_priority(choice).second + feedback);
            U_min.update(choice, U_min.get_priority(choice).second - feedback);
        }
    }

    int choose()
    {

        current_round++;
        if (current_round % (int) _eta == 0) {
            double sum = U_max.heap_sum();
            sum += L_max.heap_sum();
            L_max.normalize(sum);
            L_min.normalize(sum);
            U_max.normalize(sum);
            U_min.normalize(sum);
        } 

        if (current_round % lower_update_interval == 0)
        {
            // Add noise to all arms (Except the upper ones, but that's fine)
            for (int index : lower_indices)
            {
                double current_priority = L_max.get_priority(index).second;
                // Catch up with expected value for each skipped round, then add extra noise
                double noise = _exponential_distribution(_gen) + ((1. / _eta) * lower_update_interval);
                double new_priority = current_priority + noise;
                // Idea, put it into U already now if its better
                L_max.update(index, new_priority);
                L_min.update(index, -new_priority);
            }
            // Now that we have updated weights, we rebalance the heaps
            // While the top element of L is greater than the minimum element of U, we swap them
            while (L_max.top().priority > (1. - U_min.top().priority))
            {
                int L_top = L_max.top().key;
                double L_top_priority = L_max.top().priority;
                L_max.pop();
                // Pop it from min also
                L_min.update(L_top, 1);
                L_min.pop();
                // Work on upper
                int U_top = L_min.top().key;
                double U_top_priority = U_min.top().priority;
                U_min.pop();
                // Pop from max also
                U_max.update(U_top, U_max.top().priority + 1);
                U_max.pop();
            }
        }
        if (current_round % upper_update_interval == 0)
        {
            for (int index : upper_indices)
            {
                double current_priority = U_max.get_priority(index).second;
                double new_priority = current_priority + _exponential_distribution(_gen);

                U_max.update(index, new_priority);
                U_min.update(index, -new_priority);
            }
        }
        return U_max.top().key;
    }
    std::vector<int> choose(int K)
    {
        return std::vector<int>(_k, 1);
    }
    void give_reward(int choice, double feedback)
    {
        U_max.update(choice, U_max.top().priority + feedback);
    }

    void give_reward(std::vector<int> &indices, std::vector<double> &rewards)
    {
    }
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_FPL_BUCKETS_H