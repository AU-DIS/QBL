
#include "../csv-parser/include/csv.hpp"
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_RESULT_WRITER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_RESULT_WRITER_H

using namespace csv;
using namespace std;

static void write_results(vector<vector<double>> &regret_matrix, const string& metadata, vector<string> &descriptions, const string& path) {
    std::ofstream of;
    std::cout << "result writer Path:"  << path << std::endl;
    of.open(path, std::ios::out | std::ios::trunc);
    //of.open(path + "_reduced.txt", std::ios::out | std::ios::trunc);
    auto writer = make_csv_writer(of);
    writer << vector<string>{metadata};
    for (int i = 0; i < regret_matrix.size(); i++) {
        //writer << vector<string>{descriptions[i]};
        writer << regret_matrix[i];
    }
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_RESULT_WRITER_H
