
#include <iostream>
#include <algorithm>
#include "DepRoundALIASStrategy.h"
#include "random"
#include "../../utilities/random_gen.h"
#include <iterator>

std::vector<int> DepRoundALIASStrategy::dependent_weighted_choices(std::vector<double> probabilities, int m)
{
    std::mt19937 gen = random_gen();
    std::uniform_real_distribution<double> uniform(0, 1);

    std::vector<double> w = probabilities;
    std::uniform_real_distribution<double> dist(0, 1);

    // Added from stackoverflow: https://stackoverflow.com/questions/53632441/c-sampling-from-discrete-distribution-without-replacement

    std::vector<int> return_indices;
    return_indices.reserve(m);
    std::vector<double> vals;
    std::generate_n(std::back_inserter(vals), w.size(), [&dist, &gen]()
                    { return dist(gen); });
    std::transform(vals.begin(), vals.end(), w.begin(), vals.begin(), [&](auto r, auto w)
                   { return std::pow(r, 1. / w); });
    std::vector<std::pair<double, int>> valIndices;
    size_t index = 0;
    std::transform(vals.begin(), vals.end(), std::back_inserter(valIndices), [&index](auto v)
                   { return std::pair<double, size_t>(v, index++); });
    std::sort(valIndices.begin(), valIndices.end(), [](auto x, auto y)
              { return x.first > y.first; });
    std::vector<int> samples;
    std::transform(valIndices.begin(), valIndices.end(), std::back_inserter(samples), [](auto v)
                   { return v.second; });
    for (int i=0; i < m; i++) {
        return_indices.push_back(samples[i]);
    }

    //return_indices = samples;

    /*for (auto iter : samples) {
        std::cout << iter << " ";
    }
    std::cout << std::endl;*/
 /*
    int K = probabilities.size();


    std::vector<int> candidates;
    candidates.reserve(K);
    for (int i = 0; i < K; i++) candidates.push_back(i);

    std::vector<int> return_indices;
    std::vector<int> reserve;
    return_indices.reserve(m);

    while (candidates.size() > 1) {
        std::shuffle(std::begin(candidates), std::end(candidates), gen);
        int i = candidates.back();
        candidates.pop_back();

        int j = candidates.back();
        candidates.pop_back();

        double alpha = 1 - probabilities[i] < probabilities[j] ? 1 - probabilities[i] : probabilities[j];
        double beta = probabilities[i] < 1 - probabilities[j] ? probabilities[i] : 1 - probabilities[j];

        double r = uniform(gen);

        if (r > beta / (alpha + beta)) {
            probabilities[i] += alpha;
            probabilities[j] -= alpha;
        } else {
            probabilities[i] -= beta;
            probabilities[j] += beta;
        }

        if (probabilities[i] >= 0.99999) {return_indices.push_back(i);}
        else if (probabilities[i] > 0.00000) candidates.push_back(i);
        else reserve.push_back(i);

        if (probabilities[j] >= 0.99999) {return_indices.push_back(j);}
        else if (probabilities[j] > 0.00000) candidates.push_back(j);
        else reserve.push_back(j);

    }
    while (return_indices.size() < m) {
        //std::cout << "still wrong " << return_indices.size() << std::endl;
        //if reserve.back() =! 0
        return_indices.push_back(reserve.back());
        reserve.pop_back();
    }*/
    return return_indices;
};