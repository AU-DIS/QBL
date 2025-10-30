
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATASET_DATALOADERS_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATASET_DATALOADERS_H
#include <iostream>
# include "../csv-parser/include/csv.hpp"
#include <map>


using namespace csv;

static std::vector<std::vector<double>> load_choice_reward_dataset(const std::string &path) {
    CSVReader reader(path);
    int max_id = 0;
    std::map<int, std::vector<double>> id_to_obs;
    std::vector<std::vector<double>> data_matrix;
    for (CSVRow &row: reader) {
        int k = row["movieId"].get<int>() - 1;
        double v = row["rating"].get<double>();

        if (k > max_id) {
            max_id = k;
        }
        if (id_to_obs.find(k) != id_to_obs.end()) {
            id_to_obs[k].push_back(v);
        } else {
            std::vector<double> obs;
            obs.push_back(v);
            id_to_obs[k] = obs;
        }
    }

    data_matrix.reserve(max_id);
for (int i = 0; i < max_id; i++) {
        data_matrix.push_back(id_to_obs[i]);
    }
    return data_matrix;
}
#endif //EFFICIENT_MULTI_ARMED_BANDITS_DATASET_DATALOADERS_H