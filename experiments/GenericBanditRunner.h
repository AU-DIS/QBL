

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_GENERICBANDITRUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_GENERICBANDITRUNNER_H
#include "../datasets/dataset_simple_adversarial.h"
#include "../datasets/dataset_simple_stochastic.h"
#include "../utilities/weight_exporter.cpp"
#include "../algorithms/Exp3Bandit/Exp31.h"
#include "../algorithms/Exp3Bandit/Exp3.h"
#include "../algorithms/FPL/FPLVectorWeightStrategy.h"
#include "../algorithms/FPL/NaiveRandomGenStrategy.h"
#include "../algorithms/FPL/FPL.h"
#include "../algorithms/UCB/UCB1.h"
#include "../algorithms/Tsallis-INF/TsallisINF.h"
#include "../algorithms/Tsallis-INF/IW.h"
#include "../algorithms/Tsallis-INF/RV.h"
#include "../runner.h"
#include "../algorithms/Uniformbandit.h"
#include "../utilities/result_writer.h"

template <typename Dataset>
void run_generic_experiment(Dataset d, int K = 10, int rounds = 100, int averages=1, const std::string& out_path="/tmp/out") {
    //auto d = Dataset_movielens("../datasets/data_directory/movielens.csv", 4);
    FPLVectorWeightStrategy fpl_ws(K);
    NaiveRandomGenStrategy fpl_rs(K, 0.2);

    Exp3 exp3(K, 0.1);
    Exp31 exp31(K);

    IW iw;
    TsallisINF tsallis_iw(K, iw);
    RV rv;
    TsallisINF tsallis_rv(K, rv);

    FPL fpl(fpl_ws, fpl_rs);

    Exp3 ucb_exp3_bandit(K, 0.1);
    UCB1 ucb_exp3(10, ucb_exp3_bandit);

    FPLVectorWeightStrategy fpl_ucb_ws(K);
    NaiveRandomGenStrategy fpl_ucb_rs(K, 0.2);
    FPL ucb_fpl_bandit(fpl_ucb_ws, fpl_ucb_rs);
    UCB1 ucb_fpl(10, ucb_fpl_bandit);

    IW iw_ucb;
    TsallisINF ucb_tsallis_iw_bandit(K, iw_ucb);
    RV rv_ucb;
    TsallisINF ucb_tsallis_rv_bandit(K, rv_ucb);

    UCB1 ucb_tsallis_iw(10, ucb_tsallis_iw_bandit);
    UCB1 ucb_tsallis_rv(10, ucb_tsallis_rv_bandit);

    Uniformbandit uni(K);

    std::vector<double> exp3_regrets(rounds);
    std::vector<double> exp31_regrets(rounds);
    std::vector<double> tsallis_iw_regrets(rounds);
    std::vector<double> tsallis_rv_regrets(rounds);
    std::vector<double> fpl_regrets(rounds);

    std::vector<double> ucb_exp3_regrets(rounds);
    std::vector<double> ucb_fpl_regrets(rounds);
    std::vector<double> ucb_tsallis_iw_regrets(rounds);
    std::vector<double> ucb_tsallis_rv_regrets(rounds);

    std::vector<double> uni_regrets(rounds);

    for (int i = 0; i < averages; i++) {
        auto exp3_run = basic_runner(exp3, d, rounds);
        auto exp31_run = basic_runner(exp31, d, rounds);
        auto tsallis_iw_run = basic_runner(tsallis_iw, d, rounds);
        auto tsallis_rv_run = basic_runner(tsallis_rv, d, rounds);
        auto fpl_run = basic_runner(fpl, d, rounds);
        auto uni_run = basic_runner(uni, d, rounds);

        auto ucb_exp3_run = basic_runner(ucb_exp3, d, rounds);
        auto ucb_fpl_run = basic_runner(ucb_fpl, d, rounds);
        auto ucb_tsallis_iw_run = basic_runner(ucb_tsallis_iw, d, rounds);
        auto ucb_tsallis_rv_run = basic_runner(ucb_tsallis_rv, d, rounds);


        for (int round = 0; round < rounds; round++) {
            exp3_regrets[round] += exp3_run[round];
            exp31_regrets[round] += exp31_run[round];
            tsallis_iw_regrets[round] += tsallis_iw_run[round];
            tsallis_rv_regrets[round] += tsallis_rv_run[round];
            fpl_regrets[round] += fpl_run[round];
            uni_regrets[round] += uni_run[round];

            ucb_exp3_regrets[round] += ucb_exp3_run[round];
            ucb_fpl_regrets[round] += ucb_fpl_run[round];
            ucb_tsallis_iw_regrets[round] += ucb_tsallis_iw_run[round];
            ucb_tsallis_rv_regrets[round] += ucb_tsallis_rv_run[round];

        }
    }
    for (auto &v : exp3_regrets) v /= averages;
    for (auto &v : exp31_regrets) v /= averages;
    for (auto &v : tsallis_iw_regrets) v /= averages;
    for (auto &v : tsallis_rv_regrets) v /= averages;
    for (auto &v : fpl_regrets) v /= averages;
    for (auto &v : uni_regrets) v /= averages;


    for (auto &v : ucb_exp3_regrets) v /= averages;
    for (auto &v : ucb_fpl_regrets) v /= averages;
    for (auto &v : ucb_tsallis_iw_regrets) v /= averages;
    for (auto &v : ucb_tsallis_rv_regrets) v /= averages;

    std::vector<std::vector<double>> data_matrix;
    data_matrix.push_back(exp3_regrets);
    data_matrix.push_back(exp31_regrets);
    data_matrix.push_back(tsallis_iw_regrets);
    data_matrix.push_back(tsallis_rv_regrets);
    data_matrix.push_back(fpl_regrets);


    data_matrix.push_back(ucb_exp3_regrets);
    data_matrix.push_back(ucb_fpl_regrets);
    data_matrix.push_back(ucb_tsallis_iw_regrets);
    data_matrix.push_back(ucb_tsallis_rv_regrets);

    data_matrix.push_back(uni_regrets);


    // MUST CONTAIN ENDING COMMA
    auto description = ",";
    auto metadata =
            description +
            std::to_string(K) + ","
            + std::to_string(rounds) + ","
            + std::to_string(d.expected_value()) + ","
            + "Exp3,Exp3.1,Tsallis-INF (IW),Tsallis-INF(RV),FPL,UCB1 (Exp3), UCB1 (FPL), UCB1 (Tsallis-INF IW), UCB1 (Tsallis-INF RV),Uniform,";
    auto descriptions = std::vector<string>{
            "Exp3",
            "Exp3.1",
            "Tsallis-INF (IW)",
            "Tsallis-INF (RV)",
            "FPL",
            "UCB1 (Exp3)",
            "UCB1 (FPL)",
            "UCB1 (IW)",
            "UCB1 (RV)",
            "Uniform"
    };
    write_results(data_matrix, metadata, descriptions, out_path);
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_GENERICBANDITRUNNER_H
