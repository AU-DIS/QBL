

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_FPLCOMPARISONRUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_FPLCOMPARISONRUNNER_H

#include <thread>
#include "../algorithms/FPL/FPL.h"
#include "../algorithms/FPL/FPL_toplog.h"
#include "../algorithms/FPL/FPL_unnormalized.h"
#include "../algorithms/FPL/FPL_hash.h"
#include "../algorithms/FPL/QBL.h"
#include "../algorithms/Exp3Bandit/Exp3Tor.h"
#include "../algorithms/Exp3Bandit/Exp3IXTor.h"
#include "../algorithms/Exp3Bandit/Exp3Light.h"
#include "../algorithms/Exp3Bandit/Exp3_heap.h"
#include "../algorithms/Exp3Bandit/Exp3LightHeap.h"
#include "../datasets/dataset.h"

template <typename Dataset>
void run_fpl_adversarial_experiment(Dataset &d, int k, int rounds, int averages, double gap,
                                    const std::string &out_path = "/tmp/out")
{

    std::vector<double> fpl_original_regrets(rounds);
    std::vector<double> fpl_new_regrets(rounds);
    std::vector<double> exp3_regrets(rounds);
    std::vector<double> exp3tor_regrets(rounds);
    std::vector<double> exp3IXtor_regrets(rounds);
    std::vector<double> uniform_regrets(rounds);
    std::vector<double> exp3light_regrets(rounds);
    std::vector<double> exp3heap_regrets(rounds);
    std::vector<double> exp3lightheap_regrets(rounds);

    auto baseline = "FPL";
    auto uniform = "Uniform";
    auto compared = "QBL";
    auto exp3_tor = "Exp3tor";
    auto exp3_IXtor = "Exp3IXtor";
    auto exp3_comp = "Exp3";
    auto exp3_light = "Exp3Light";
    auto exp3_heap = "Exp3 Heap";
    auto exp3_light_heap = "Exp3Light Heap";

    for (int i = 0; i < averages; i++)
    {

        std::vector<std::vector<double>> data_matrix = d.generate();

        double exp_eta = 10;
        FPL fpl(k, exp_eta);
        // FPL_hash fpl_new(k, 0.9, rounds);
        // FPL_toplog fpl_new(k, eta);

        QBL fpl_new(k, 5);
        double gamma = 0.1;
        double eta = 0.1;
        Exp3 exp3(k, gamma);
        Exp3Tor exp3tor(k,eta);
        Exp3IXTor exp3IXtor(k, eta);
        Exp3Light exp3light(k, eta);
        Exp3_heap exp3heap(k, eta);
        Exp3LightHeap exp3lightheap(k, eta);



        Uniformbandit uni(k);
        // FPL_hash fpl_new(k, eta, rounds);
        // QBL fpl_new(k, eta);

        std::vector<double> fpl_original_run;
        std::thread t1(basic_runner<FPL>, std::ref(fpl), std::ref(data_matrix), rounds, std::ref(fpl_original_run));

        std::vector<double> fpl_new_run;
        std::thread t2(basic_runner<QBL>, std::ref(fpl_new), std::ref(data_matrix), rounds, std::ref(fpl_new_run));

        std::vector<double> exp3_run;
        std::thread t3(basic_runner<Exp3>, std::ref(exp3), std::ref(data_matrix), rounds, std::ref(exp3_run));

        std::vector<double> uniform_run;
        std::thread t4(basic_runner<Uniformbandit>, std::ref(uni), std::ref(data_matrix), rounds,
                       std::ref(uniform_run));

        std::vector<double> exp3tor_run;
        std::thread t5(basic_runner<Exp3Tor>, std::ref(exp3tor), std::ref(data_matrix), rounds,
                      std::ref(exp3tor_run));
        std::vector<double> exp3IXtor_run;
        std::thread t6(basic_runner<Exp3IXTor>, std::ref(exp3IXtor), std::ref(data_matrix), rounds,
                       std::ref(exp3IXtor_run));

        std::vector<double> exp3Light_run;
        std::thread t7(basic_runner<Exp3Light>, std::ref(exp3light), std::ref(data_matrix), rounds,
                       std::ref(exp3Light_run));

        std::vector<double> exp3Heap_run;
        std::thread t8(basic_runner<Exp3_heap>, std::ref(exp3heap), std::ref(data_matrix), rounds,
                       std::ref(exp3Heap_run));

        std::vector<double> exp3LightHeap_run;
        std::thread t9(basic_runner<Exp3LightHeap>, std::ref(exp3lightheap), std::ref(data_matrix), rounds,
                       std::ref(exp3LightHeap_run));


        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();

        for (int round = 0; round < rounds; round++)
        {
            fpl_original_regrets[round] += fpl_original_run[round];
            fpl_new_regrets[round] += fpl_new_run[round];
            exp3_regrets[round] += exp3_run[round];
            uniform_regrets[round] += uniform_run[round];
            exp3tor_regrets[round] += exp3tor_run[round];
            exp3IXtor_regrets[round] += exp3IXtor_run[round];
            exp3light_regrets[round] += exp3Light_run[round];
            exp3heap_regrets[round] += exp3Heap_run[round];
            exp3lightheap_regrets[round] += exp3LightHeap_run[round];
        }
    }
    for (auto &v : fpl_original_regrets)
        v /= averages;

    for (auto &v : fpl_new_regrets)
        v /= averages;
    for (auto &v : exp3_regrets)
        v /= averages;
    for (auto &v : uniform_regrets)
        v /= averages;
    for (auto &v : exp3tor_regrets)
        v /= averages;
    for (auto &v : exp3IXtor_regrets)
        v /= averages;
    for (auto &v : exp3light_regrets)
        v /= averages;
    for (auto &v : exp3heap_regrets)
        v /= averages;
    for (auto &v : exp3lightheap_regrets)
        v /= averages;

    std::vector<std::vector<double>> result_matrix;
    result_matrix.push_back(fpl_original_regrets);

    result_matrix.push_back(fpl_new_regrets);

    result_matrix.push_back(exp3_regrets);
    result_matrix.push_back(uniform_regrets);

    result_matrix.push_back(exp3tor_regrets);
    result_matrix.push_back(exp3IXtor_regrets);
    result_matrix.push_back(exp3light_regrets);
    result_matrix.push_back(exp3heap_regrets);
    result_matrix.push_back(exp3lightheap_regrets);

    // MUST CONTAIN ENDING COMMA
    auto description = ",";
    auto metadata =
        description +
        std::to_string(k) + "," + std::to_string(rounds) + "," + std::to_string(gap) + "," +
        baseline + "," +
        compared + "," +
        exp3_comp + ","+
        uniform + "," +
        exp3_tor + "," +
        exp3_IXtor + "," +
        exp3_light + "," +
        exp3_heap + "," +
        exp3_light_heap + ",";
    auto descriptions = std::vector<string>{
        baseline,
        compared,
        exp3_comp,
        uniform,
        exp3_tor,
        exp3_IXtor,
        exp3_light,
        exp3_heap,
        exp3_light_heap};
    write_results(result_matrix, metadata, descriptions, out_path);
}

#endif // EFFICIENT_MULTI_ARMED_BANDITS_FPLCOMPARISONRUNNER_H
