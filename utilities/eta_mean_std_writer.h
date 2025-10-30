
#include "../csv-parser/include/csv.hpp"
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_ETA_MEAN_STD_WRITER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_ETA_MEAN_STD_WRITER_H

using namespace csv;
using namespace std;

static void write_results(vector<vector<double>> &data_matrix, const vector<string> &comments, const vector<string> &header, const string& path) {
    std::ofstream of;
    of.open(path, std::ios::out | std::ios::trunc);
    auto writer = make_csv_writer(of);
    writer << comments;
    writer << header;
    for (int i = 0; i < data_matrix.size(); i++) {
        writer << data_matrix[i];
    }
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_ETA_MEAN_STD_WRITER_H
