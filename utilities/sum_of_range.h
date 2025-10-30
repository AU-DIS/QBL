
#include "../csv-parser/include/csv.hpp"
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_SUM_OF_RANGE_H
#define EFFICIENT_MULTI_ARMED_BANDITS_SUM_OF_RANGE_H

using namespace csv;
using namespace std;

static double sum_of_range(vector<double> &arr, int start, int end) {
    double res = 0;
    for (int i = start; i < end; i++) {
        res += arr[i];
    }
    return res;
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_SUM_OF_RANGE_H
