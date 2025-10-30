

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_EXPLORENOMORERUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_EXPLORENOMORERUNNER_H
#include "../datasets/dataset_explore_no_more.h"
#include "../algorithms/Exp3Bandit/Exp3Tor.h"
#include "../algorithms/Exp3Bandit/Exp3IXTor.h"
#include "../utilities/standard_deviation.cpp"
#include "../utilities/eta_mean_std_writer.h"
#include "../utilities/sum_of_range.h"
#include "../runner.h"
#include <stdlib.h>
#include <numeric>

void run_explore_no_more_experiment(const std::string& out_path="/tmp/out") {
    int K = 10;
    int rounds = 1000000;
    int repititions = 50;

    std::vector<std::vector<double>> data_matrix;
    for (int eta_index = 0; eta_index <= 10; eta_index++) {
        //auto eta = pow(10, ((double)eta_index)/2.5-2);
        auto eta = pow(10, ((double)eta_index)/2.5-5);
        std::vector<double> exp3_regret_T_half(repititions, 0);
        std::vector<double> exp3_regret_T(repititions, 0);
        std::vector<double> exp3IX_regret_T_half(repititions, 0);
        std::vector<double> exp3IX_regret_T(repititions, 0);
        for (int i = 0; i < repititions; i++) {
            auto d = dataset_explore_no_more(rounds);
            auto data_matrix = d.generate();
            Exp3Tor exp3(K, eta);
            Exp3IXTor exp3IX(K, eta, eta/2);
            std::vector<double> exp3_regrets;
            basic_runner(exp3, data_matrix, rounds, exp3_regrets);
            std::vector<double> exp3IX_regrets;
            basic_runner(exp3IX, data_matrix, rounds, exp3IX_regrets);
            exp3_regret_T_half[i] = sum_of_range(exp3_regrets, 0, rounds/2);
            exp3_regret_T[i] = sum_of_range(exp3_regrets, 0, rounds);
            exp3IX_regret_T_half[i] = sum_of_range(exp3IX_regrets, 0, rounds/2);
            exp3IX_regret_T[i] = sum_of_range(exp3IX_regrets, 0, rounds);
        }
        auto [mean_exp3_T_half, std_exp3_T_half] = standard_deviation(exp3_regret_T_half);
        auto [mean_exp3_T, std_exp3_T] = standard_deviation(exp3_regret_T);
        auto [mean_exp3IX_T_half, std_exp3IX_T_half] = standard_deviation(exp3IX_regret_T_half);
        auto [mean_exp3IX_T, std_exp3IX_T] = standard_deviation(exp3IX_regret_T);
        data_matrix.push_back({eta, mean_exp3_T_half, std_exp3_T_half, mean_exp3_T, std_exp3_T, mean_exp3IX_T_half, std_exp3IX_T_half, mean_exp3IX_T, std_exp3IX_T});
    }

    vector<string> comments {"#Explore No More", std::to_string(K), std::to_string(rounds)};
    vector<string> header {"eta","mean_regret_exp3_T_half","std_regret_exp3_T_half","mean_regret_exp3_T","std_regret_exp3_T","mean_regret_exp3IX_T_half","std_regret_exp3IX_T_half","mean_regret_exp3IX_T","std_regret_exp3IX_T"};
    write_results(data_matrix, comments, header, out_path + "/explore_no_more.csv");
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_EXPLORENOMORERUNNER_H
