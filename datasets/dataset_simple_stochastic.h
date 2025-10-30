

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATASET_SIMPLE_STOCHASTIC_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATASET_SIMPLE_STOCHASTIC_H
#include "vector"
#include "../utilities/random_gen.h"
#include "random"
#include "iostream"
#include "dataset.h"

class dataset_simple_stochastic {
private:
    std::vector<std::vector<double>> data_matrix;
    std::vector<int> iterators;
    double _p;

public:
    /// @param values K
    /// @param values number_of_rounds number of rounds
    /// @param values p The reward of each arm is binary and Bernoulli distributed with probability p
    dataset_simple_stochastic(int K, int number_of_rounds, double p) {
        auto gen = random_gen();
        _p = p;
        auto weights = {p, 1 - p};
        std::discrete_distribution<int> d(weights.begin(), weights.end());

        for (int i = 0; i < K; i++) {
            iterators.push_back(0);
            std::vector<double> column;
            for (int j = 0; j < number_of_rounds; j++) {
                auto w = d(gen);
                column.push_back(w);
            }
            data_matrix.push_back(column);
        }
        reset();
    }
    void reset() {
        for (auto &i : iterators) i = 0;
    }
    double expected_value() {
        return _p;
    }

    auto feedback(int choice) {
        struct return_values {
            double feedback, regret;
        };
        auto index = iterators[choice];
        auto reward = data_matrix[choice][index];
        iterators[choice] += 1;
        return return_values{reward, 1 - reward};
    }
};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_DATASET_SIMPLE_STOCHASTIC_H
