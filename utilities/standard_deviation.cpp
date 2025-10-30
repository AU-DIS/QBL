//
// Created by hhhha on 09-03-2022.
//

#include "vector"
#include "cmath"

auto standard_deviation(std::vector<double> arr) {
    struct result_struct {
        double mean, std;
    };
    double sum, mean, std = 0.0;
    double size = arr.size();

    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }

    mean = sum / size;

    for(int i = 0; i < size; i++) {
        std += pow(arr[i] - mean, 2);
    }

    return result_struct { mean, sqrt(std/size) };
}