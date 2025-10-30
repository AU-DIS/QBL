
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_ADVERSARIALEXPERIMENTRUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_ADVERSARIALEXPERIMENTRUNNER_H

#include "../algorithms/Exp3m/DepRoundALIASStrategy.h"
#include "../algorithms/Exp3m/Exp3m.h"
#include "../algorithms/Exp3m/Exp31m.h"
#include "../algorithms/Exp3m/Exp31m_2.h"
#include "../algorithms/Exp3m/Exp3m_heap.h"
#include "../algorithms/Exp3m/Exp3light_m_heap.h"
#include "../algorithms/Uniformbandit.h"
#include "../utilities/result_writer.h"
#include "../runner.h"
#include "../algorithms/FPL/FPL.h"
#include "../algorithms/FPL/QBL.h"
#include "../algorithms/Exp3Bandit/Exp3.h"
#include "../algorithms/Exp3Bandit/Exp31.h"
#include "../algorithms/Tsallis-INF/Tsallis_RV.h"
#include "../algorithms/Tsallis-INF/Tsallis_IW.h"
#include "../algorithms/UCB/UCB1.h"
#include "../datasets/dataset.h"
#include "set"

// template<typename Dataset>
void run_adversarial_weight_experiment(Dataset &d, int k, int rounds, double gap,
                                       int averages = 50, const std::string &regret_out_path = "/tmp/out",
                                       const std::string &weight_out_path = "/tmp/out",
                                       const std::string &algorithm = "Exp3")
{
    std::vector<std::vector<double>> regrets;
    std::vector<double> bandit_regrets(rounds, 0);
    std::vector<double> uniform_regrets(rounds, 0);

    std::vector<std::vector<double>> weights_at_r;
    int interval_changes = std::floor(std::log2(rounds) / std::log2(3.2));
    for (int i = 0; i < interval_changes; i++)
    {
        std::vector<double> weights(k, 0);
        weights_at_r.push_back(weights);
    }

    for (int avg = 0; avg < averages; avg++)
    {
        std::vector<std::vector<double>> data_matrix = d.generate();

        FPL fpl(k, 10);
        Uniformbandit uni(k);

        //        Exp3 fpl(k, 0.1);

        int multiple = 1;
        int interval = 0;
        for (int round = 0; round < rounds; round++)
        {
            double threshold = pow(gap, multiple);
            if (round > threshold)
            {
                auto w = *fpl.get_weights();
                for (int j = 0; j < k; j++)
                {
                    weights_at_r[interval][j] += (w[j] / averages);
                }
                interval++;
                multiple++;
            }
            auto bandit_choice = fpl.choose();
            auto bandit_reward = data_matrix[bandit_choice][round];
            auto uniform_choice = uni.choose();
            auto uni_reward = data_matrix[uniform_choice][round];
            double max_element = 0;
            for (auto &arm : data_matrix)
            {
                if (arm[round] >= max_element)
                    max_element = arm[round];
            }
            double bandit_regret = max_element - bandit_reward;
            double uniform_regret = max_element - uni_reward;
            fpl.give_reward(bandit_choice, bandit_reward);
            bandit_regrets[round] += (bandit_regret / averages);
            uniform_regrets[round] += (uniform_regret / averages);
        }
    }

    regrets.push_back(bandit_regrets);
    regrets.push_back(uniform_regrets);

    // MUST CONTAIN ENDING COMMA
    auto description = ",";
    auto metadata =
        description +
        std::to_string(k) + "," + std::to_string(rounds) + "," + std::to_string(gap) + "," + algorithm + ",uniform,";
    auto descriptions = std::vector<std::string>{
        algorithm,
        "Uniform"};

    auto weights_metadata = "";
    auto weights_descriptions = std::vector<std::string>{
        ""};

    write_results(regrets, metadata, descriptions, regret_out_path);
    write_results(weights_at_r, weights_metadata, weights_descriptions, weight_out_path);
}

// template<typename Dataset>
void run_adversarial_exp3m_experiment(Dataset &d, int m, int k, int rounds, int averages,
                                      double gap,
                                      const std::string &out_path = "/tmp/out")
{

    std::vector<double> uniform_regrets(rounds);
    std::vector<double> qbl_regrets(rounds);
    std::vector<double> fpl_regrets(rounds);
    std::vector<double> exp3m_regrets(rounds);
    std::vector<double> exp3m_heap_regrets(rounds);
    std::vector<double> exp3light_m_heap_regrets(rounds);



    for (int i = 0; i < averages; i++)
    {
        std::vector<std::vector<double>> data_matrix = d.generate();

        Uniformbandit uni(k);
        Exp3m b(m, k, 0.1);
        Exp3m_heap b_heap(m, k, 0.1);
        QBL qbl(k, 5);
        FPL fpl(k, 10);
        Exp3light_m_heap b_lightheap(m,k,0.1);

        /*
                Exp3m_heap b_heap(m, k, 0.1);
        */

        std::vector<double> uniform_run;
        std::thread t1(single_top_k_runner<Uniformbandit>, std::ref(uni), std::ref(data_matrix), rounds, k, m, std::ref(uniform_run));

        std::vector<double> exp3m_run;
        std::thread t2(top_k_runner<Exp3m>, std::ref(b), std::ref(data_matrix), rounds, k, m,
                       std::ref(exp3m_run));

        std::vector<double> qbl_run;
        std::thread t3(top_k_runner<QBL>, std::ref(qbl), std::ref(data_matrix), rounds, k, m, std::ref(qbl_run));

        std::vector<double> fpl_run;
        std::thread t4(top_k_runner<FPL>, std::ref(fpl), std::ref(data_matrix), rounds, k, m, std::ref(fpl_run));

        std::vector<double> exp3m_heap_run;
        std::thread t5(top_k_runner<Exp3m_heap>, std::ref(b_heap), std::ref(data_matrix), rounds, k, m,
                       std::ref(exp3m_heap_run));

        std::vector<double> exp3light_m_heap_run;
        std::thread t6(top_k_runner<Exp3light_m_heap>, std::ref(b_lightheap), std::ref(data_matrix), rounds, k, m,
                       std::ref(exp3light_m_heap_run));


        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();

        for (int round = 0; round < rounds; round++)
        {
            uniform_regrets[round] += uniform_run[round];
            exp3m_regrets[round] += exp3m_run[round];
            exp3m_heap_regrets[round] += exp3m_heap_run[round];
            qbl_regrets[round] += qbl_run[round];
            fpl_regrets[round] += fpl_run[round];
            exp3light_m_heap_regrets[round] += exp3light_m_heap_run[round];
        }
    }
    for (auto &v : uniform_regrets)
        v /= averages;
    for (auto &v : exp3m_regrets)
        v /= averages;
    for (auto &v : exp3m_heap_regrets)
        v /= averages;
    for (auto &v : qbl_regrets)
        v /= averages;
    for (auto &v : fpl_regrets)
        v /= averages;
    for (auto &v : exp3light_m_heap_regrets)
        v /= averages;

    std::vector<std::vector<double>> result_matrix;
    result_matrix.push_back(uniform_regrets);
    result_matrix.push_back(exp3m_regrets);
    result_matrix.push_back(exp3m_heap_regrets);
    result_matrix.push_back(fpl_regrets);
    result_matrix.push_back(qbl_regrets);
    result_matrix.push_back(exp3light_m_heap_regrets);

    // MUST CONTAIN ENDING COMMA
    auto description = ",";
    auto metadata =
        description +
        std::to_string(k) + "," +
        std::to_string(rounds) + "," +
        std::to_string(gap) + "," +
        "Uniform,Exp3m,Exp3m Heap,FPL,QBL,Exp3Light.M Heap";
    //"Uniform,Exp3m";
    auto descriptions = std::vector<string>{
        "Uniform,Exp3m,Exp3m Heap,FPL,QBL,Exp3Light.M Heap"};
    //"Uniform,Exp3m"};
    write_results(result_matrix, metadata, descriptions, out_path);
}

void run_adversarial_experiment(Dataset &d, int k, int rounds, int averages, double gap,
                                const std::string &out_path = "/tmp/out")
{

    std::vector<double> exp3_regrets(rounds);
    std::vector<double> exp31_regrets(rounds);

    std::vector<double> ucb_exp3_regrets(rounds);
    std::vector<double> tsallis_iw_regrets(rounds);
    std::vector<double> tsallis_rv_regrets(rounds);

    std::vector<double> fpl_regrets(rounds);
    std::vector<double> ucb_fpl_regrets(rounds);
    std::vector<double> uniform_regrets(rounds);

    for (int i = 0; i < averages; i++)
    {
        std::vector<std::vector<double>> data_matrix = d.generate();
        Exp3 exp3(k, 0.1);

        Tsallis_IW tsallis_iw(k);
        Tsallis_RV tsallis_rv(k);

        Exp3 ucb_exp3_bandit(k, 0.1);
        UCB1 ucb_exp3(10, ucb_exp3_bandit);

        Uniformbandit uni(k);

        FPL fpl(k, 10);

        FPL ucb_fpl_bandit(k, 10);
        UCB1 ucb_fpl(10, ucb_fpl_bandit);

        Exp31 exp31(k);

        std::vector<double> exp3_run;
        std::thread t1(basic_runner<Exp3>, std::ref(exp3), std::ref(data_matrix), rounds, std::ref(exp3_run));

        std::vector<double> exp31_run;

        std::thread t2(basic_runner<Exp31>, std::ref(exp31), std::ref(data_matrix), rounds,
                       std::ref(exp31_run));

        std::vector<double> fpl_run;
        // std::thread t3(basic_tsallis_runner<FPL<FPLVectorWeightStrategy, NaiveRandomGenStrategy>>, std::ref(fpl),
        std::thread t3(basic_runner<FPL>, std::ref(fpl),
                       std::ref(data_matrix), rounds, std::ref(fpl_run));

        std::vector<double> ucb_fpl_run;
        std::thread t4(basic_runner<UCB1<FPL>>,
                       std::ref(ucb_fpl), std::ref(data_matrix), rounds, std::ref(ucb_fpl_run));

        std::vector<double> tsallis_iw_run;
        std::thread t5(basic_runner<Tsallis_IW>, std::ref(tsallis_iw), std::ref(data_matrix), rounds,
                       std::ref(tsallis_iw_run));

        std::vector<double> tsallis_rv_run;
        std::thread t6(basic_runner<Tsallis_RV>, std::ref(tsallis_rv), std::ref(data_matrix), rounds,
                       std::ref(tsallis_rv_run));

        std::vector<double> ucb_exp3_run;
        std::thread t7(basic_runner<UCB1<Exp3>>, std::ref(ucb_exp3), std::ref(data_matrix), rounds,
                       std::ref(ucb_exp3_run));

        std::vector<double> uniform_run;
        std::thread t8(basic_runner<Uniformbandit>, std::ref(uni), std::ref(data_matrix), rounds,
                       std::ref(uniform_run));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();

        for (int round = 0; round < rounds; round++)
        {
            exp3_regrets[round] += exp3_run[round];
            exp31_regrets[round] += exp31_run[round];
            fpl_regrets[round] += fpl_run[round];
            ucb_fpl_regrets[round] += ucb_fpl_run[round];
            tsallis_iw_regrets[round] += tsallis_iw_run[round];
            tsallis_rv_regrets[round] += tsallis_rv_run[round];
            ucb_exp3_regrets[round] += ucb_exp3_run[round];
            uniform_regrets[round] += uniform_run[round];
        }
    }

    for (auto &v : exp3_regrets)
        v /= averages;
    for (auto &v : exp31_regrets)
        v /= averages;
    for (auto &v : ucb_fpl_regrets)
        v /= averages;
    for (auto &v : fpl_regrets)
        v /= averages;
    for (auto &v : tsallis_iw_regrets)
        v /= averages;
    for (auto &v : tsallis_rv_regrets)
        v /= averages;
    for (auto &v : ucb_exp3_regrets)
        v /= averages;
    for (auto &v : uniform_regrets)
        v /= averages;

    std::vector<std::vector<double>> result_matrix;
    result_matrix.push_back(exp3_regrets);
    result_matrix.push_back(exp31_regrets);
    result_matrix.push_back(ucb_exp3_regrets);

    result_matrix.push_back(fpl_regrets);
    result_matrix.push_back(ucb_fpl_regrets);

    result_matrix.push_back(tsallis_iw_regrets);
    result_matrix.push_back(tsallis_rv_regrets);

    result_matrix.push_back(uniform_regrets);

    // MUST CONTAIN ENDING COMMA
    auto description = ",";
    auto metadata =
        description +
        std::to_string(k) + "," + std::to_string(rounds) + "," + std::to_string(gap) + "," + "Exp3,Exp3.1,UCB1 (Exp3),FPL,UCB (FPL),Tsallis-INF (IW),Tsallis-INF (RV),Uniform,";
    auto descriptions = std::vector<string>{
        "Exp3",
        "Exp3.1",
        "UCB1 (Exp3)",
        "FPL",
        "UCB (FPL)",
        "Tsallis-INF (IW)",
        "Tsallis-INF (RV)",
        "Uniform"};
    write_results(result_matrix, metadata, descriptions, out_path);
}

#endif // EFFICIENT_MULTI_ARMED_BANDITS_ADVERSARIALEXPERIMENTRUNNER_H