

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATASET_EXPLORE_NO_MORE_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATASET_EXPLORE_NO_MORE_H
#include "vector"
#include "../utilities/random_gen.h"
#include "random"
#include "iostream"

class dataset_explore_no_more {
private:
    double _rounds;

public:
    explicit dataset_explore_no_more(int rounds) : _rounds(rounds) {};
    std::vector<std::vector<double>> generate() {
        auto delta = 0.1;
        auto gen = random_gen();
        std::discrete_distribution<int> d;

        std::vector<std::vector<double>> data_matrix;

        for (int i = 0; i < 8; i++) {
            std::vector<double> column;
            d = std::discrete_distribution<int>({0.5, 0.5});
            for (int j = 0; j < _rounds; j++) {
                auto w = d(gen);
                column.push_back(w);
            }
            data_matrix.push_back(column);
        }
        std::vector<double> ninth_column;
        d = std::discrete_distribution<int>({0.5+delta, 0.5-delta});
        for (int j = 0; j < _rounds; j++) {
            auto w = d(gen);
            ninth_column.push_back(w);
        }
        data_matrix.push_back(ninth_column);
        std::vector<double> tenth_column;
        d = std::discrete_distribution<int>({0.5+delta, 0.5-delta});
        for (int j = 0; j < _rounds/2; j++) {
            auto w = d(gen);
            tenth_column.push_back(w);
        }
        d = std::discrete_distribution<int>({0.5+delta*4, 0.5-delta*4});
        for (int j = 0; j < _rounds/2; j++) {
            auto w = d(gen);
            tenth_column.push_back(w);
        }
        data_matrix.push_back(tenth_column);

        return data_matrix;
    }
};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_DATASET_EXPLORE_NO_MORE_H
