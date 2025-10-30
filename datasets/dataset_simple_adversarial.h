

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATASET_SIMPLE_ADVERSARIAL_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATASET_SIMPLE_ADVERSARIAL_H

#include "vector"
#include "../utilities/random_gen.h"
#include "random"
#include "iostream"
#include "dataset.h"

class dataset_simple_adversarial{
private:
    std::vector<std::vector<double>> data_matrix;
    std::vector<int> iterators;
    double expected_reward;
    int number_of_distribution_changes = 0;
public:
    dataset_simple_adversarial(int K, int number_of_rounds) {
        double p = 0.5;
        auto gen = random_gen();
        std::vector<double> weights = {p, 1 - p};


        for (int i = 0; i < K; i++) iterators.push_back(0);

        for (int i = 0; i < K; i++) {
            std::vector<double> row;
            for (int j = 0; j < number_of_rounds; j++) {
                row.push_back(0.0);
            }
            data_matrix.push_back(row);
        }

        for (int column = 0; column < K; column++) {
            for (int row = 0; row < number_of_rounds; row++) {
                if ((float) rand()/RAND_MAX >= 0.999)  {
                    p = (float) rand()/RAND_MAX;
                    weights = {p, 1 - p};
                    number_of_distribution_changes += 1;
                }
                std::discrete_distribution<int> d(weights.begin(), weights.end());
                double r = d(gen);
                data_matrix[column][row] = r;
            }
        }
        reset();
        std::vector<double> uniform_weights;
        for (int i = 0; i < K; i++) uniform_weights.push_back(1.0/K);
        std::discrete_distribution<int> d(uniform_weights.begin(), uniform_weights.end());
        for (int i = 0; i < number_of_rounds; i++) {
            int choice = d(gen);
            auto index = iterators[choice];
            auto reward = data_matrix[choice][index];
            iterators[choice] += 1;
            expected_reward += reward;
        }
        expected_reward /= number_of_rounds;
        reset();

    }
    void reset() {
        iterators = std::vector<int>(data_matrix.size(), 0);
    }

    double expected_value() {
       return expected_reward;
    }
    int number_of_changes() {
        return number_of_distribution_changes;
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

#endif //EFFICIENT_MULTI_ARMED_BANDITS_DATASET_SIMPLE_ADVERSARIAL_H
