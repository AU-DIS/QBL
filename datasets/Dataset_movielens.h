

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATASET_MOVIELENS_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATASET_MOVIELENS_H


#include <vector>
#include "dataloaders.h"

/// The Movielens dataset contains 20000263 reviews across 27278 movies.
/// The dataset must be downloaded at https://www.kaggle.com/grouplens/movielens-20m-dataset?select=rating.csv
/// Remember to process it using data_processors/movielens_preprocessor.py
///
class Dataset_movielens {
private:
    std::vector<std::vector<double>> _data_matrix;
    std::vector<int> iterators;
    double expected_reward;
public:
    int K;
    Dataset_movielens(const std::string &path, double threshold) {
        _data_matrix = load_choice_reward_dataset(path);
        for (int i = 0; i < _data_matrix.size(); i++) iterators.push_back(0);
        K = _data_matrix.size();
        for (int column = 0; column < _data_matrix.size(); column++) {
            for (int row = 0; row < _data_matrix[column].size(); row++) {
                double r = _data_matrix[column][row] >= threshold ? 1 : 0;
                _data_matrix[column][row] = r;
            }
        }
        std::vector<double> column_averages;

        for (int i = 0; i < _data_matrix.size(); i++) {
            column_averages.push_back(0.0);
            for (int j = 0; j < _data_matrix[i].size(); j++) {
                column_averages[i] += _data_matrix[i][j];
            }
        }
        for (int i = 0; i < _data_matrix.size(); i++) {
            column_averages[i] /= _data_matrix[i].size();
        }

        expected_reward = 0;
        for (auto avg : column_averages) expected_reward += avg;
        expected_reward /= K;
        reset();
    }

    double expected_value() {
        return expected_reward;
    }
    void reset() {
        for (auto &i : iterators) i = 0;
    }

    auto feedback(int choice) {
        struct return_values {
            double feedback, regret;
        };
        auto index = iterators[choice];
        auto reward = _data_matrix[choice][index];
        iterators[choice] = (1 + iterators[choice]) % _data_matrix[choice].size();
        return return_values{reward, 1 - reward};
    }
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_DATASET_MOVIELENS_H
