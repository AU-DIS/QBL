
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_TSALLISCOMPARISONRUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_TSALLISCOMPARISONRUNNER_H

#include "../algorithms/Tsallis-INF/TsallisINF.h"
#include "../algorithms/Tsallis-INF/Tsallis_approx_rv.h"
#include "../algorithms/Tsallis-INF/Tsallis_IW.h"
#include "../algorithms/Tsallis-INF/Tsallis_Heap.h"
#include "../algorithms/Tsallis-INF/Tsallis_LTU.h"
#include "../algorithms/Tsallis-INF/Tsallis_RV.h"

void run_tsallis_adversarial_experiment(Dataset &d, int k, int rounds, int averages, double gap,
                                        const std::string &out_path = "/tmp/out")
{

    std::vector<double> tsallis_iw_regrets(rounds);
    std::vector<double> tsallis_rv_regrets(rounds);
    std::vector<double> tsallis_optimized_regrets(rounds);
    std::vector<double> uniform_regrets(rounds);

    for (int i = 0; i < averages; i++)
    {
        std::vector<std::vector<double>> data_matrix = d.generate();

        Tsallis_IW iw(k);
        Tsallis_RV rv(k);
        // Tsallis_LTU optimized(k);
        // Tsallis_Heap optimized(k);
        Tsallis_Heap approx(k);
        Uniformbandit uni(k);

        std::vector<double> tsallis_iw_run;
        std::thread t1(basic_runner<Tsallis_IW>, std::ref(iw), std::ref(data_matrix), rounds,
                       std::ref(tsallis_iw_run));

        std::vector<double> tsallis_rv_run;
        std::thread t2(basic_runner<Tsallis_RV>, std::ref(rv), std::ref(data_matrix), rounds,
                       std::ref(tsallis_rv_run));

        std::vector<double> uniform_run;
        std::thread t3(basic_runner<Uniformbandit>, std::ref(uni), std::ref(data_matrix), rounds,
                       std::ref(uniform_run));

        std::vector<double> tsallis_optimized_run;
        std::thread t4(basic_runner<Tsallis_Heap>, std::ref(approx), std::ref(data_matrix), rounds,
                       std::ref(tsallis_optimized_run));

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        for (int round = 0; round < rounds; round++)
        {
            tsallis_iw_regrets[round] += tsallis_iw_run[round];
            tsallis_rv_regrets[round] += tsallis_rv_run[round];
            tsallis_optimized_regrets[round] += tsallis_optimized_run[round];
            uniform_regrets[round] += uniform_run[round];
        }
    }
    for (auto &v : tsallis_iw_regrets)
        v /= averages;
    for (auto &v : tsallis_rv_regrets)
        v /= averages;
    for (auto &v : tsallis_optimized_regrets)
        v /= averages;
    for (auto &v : uniform_regrets)
        v /= averages;
    std::vector<std::vector<double>> result_matrix;
    result_matrix.push_back(tsallis_iw_regrets);
    result_matrix.push_back(tsallis_rv_regrets);
    result_matrix.push_back(tsallis_optimized_regrets);
    result_matrix.push_back(uniform_regrets);
    // MUST CONTAIN ENDING COMMA
    auto description = ",";
    auto metadata =
        description +
        std::to_string(k) + "," + std::to_string(rounds) + "," + std::to_string(gap) + "," + "Tsallis (IW),Tsallis (RV),Cumulative loss sampling,Uniform,";
    auto descriptions = std::vector<string>{
        "Tsallis (IW)",
        "Tsallis (RV)",
        "Cumulative loss sampling",
        "Uniform"};
    write_results(result_matrix, metadata, descriptions, out_path);
}

template <typename Dataset>
void run_tsallis_variance_experiment(Dataset d, int k, int rounds, int averages, double gap,
                                     const std::string &out_path = "/tmp/out")
{

    double avg_rv = 0;
    double avg_iw = 0;
    double avg_optimized = 0;

    std::vector<double> iw_losses(k, 0);
    std::vector<double> rv_losses(k, 0);
    std::vector<double> optimized_losses(k, 0);

    for (int i = 0; i < averages; i++)
    {
        std::vector<std::vector<double>> data_matrix = d.generate();

        Tsallis_RV rv(k);
        Tsallis_approx_rv optimized(k);
        Tsallis_IW iw(k);

        std::vector<double> tsallis_rv_run;
        std::thread t1(basic_runner<Tsallis_RV>, std::ref(rv), std::ref(data_matrix), rounds,
                       std::ref(tsallis_rv_run));

        std::vector<double> tsallis_optimized_run;
        std::thread t2(basic_runner<Tsallis_approx_rv>, std::ref(optimized), std::ref(data_matrix), rounds,
                       std::ref(tsallis_optimized_run));

        std::vector<double> tsallis_iw_run;
        std::thread t3(basic_runner<Tsallis_IW>, std::ref(iw), std::ref(data_matrix), rounds,
                       std::ref(tsallis_iw_run));

        t1.join();
        t2.join();
        t3.join();

        double rv_mean = 0;
        double optimized_mean = 0;
        double iw_mean = 0;
        for (int j = 0; j < k; j++)
        {
            rv_mean += rv._cumulative_losses[j];
            optimized_mean += optimized._cumulative_losses[j];
            iw_mean += iw._cumulative_losses[j];
        }
        rv_mean /= k;
        optimized_mean /= k;
        iw_mean /= k;

        double rv_variance = 0;
        double iw_variance = 0;
        double optimized_variance = 0;
        for (int j = 0; j < k; j++)
        {
            rv_variance += pow((rv._cumulative_losses[j] - rv_mean), 2);
            iw_variance += pow((iw._cumulative_losses[j] - iw_mean), 2);
            optimized_variance += pow((optimized._cumulative_losses[j] - optimized_mean), 2);
        }

        rv_variance /= (k - 1);
        iw_variance /= (k - 1);
        optimized_variance /= (k - 1);

        avg_rv += rv_variance;
        avg_iw += iw_variance;
        avg_optimized = optimized_variance;

        for (int arm = 0; arm < k; arm++)
        {
            rv_losses[arm] += rv._cumulative_losses[arm];
            iw_losses[arm] += iw._cumulative_losses[arm];
            optimized_losses[arm] += optimized._cumulative_losses[arm];
        }
    }
    avg_rv /= averages;
    avg_iw /= averages;
    avg_optimized /= averages;

    std::cout << "iw = [";
    for (int i = 0; i < k; i++)
    {
        std::cout << to_string(iw_losses[i] /= averages) << ",";
    }
    std::cout << "]";
    std::cout << std::endl;
    std::cout << "rv = [";
    for (int i = 0; i < k; i++)
    {
        std::cout << to_string(rv_losses[i] /= averages) << ",";
    }
    std::cout << "]";
    std::cout << std::endl;
    std::cout << "optimized = [";
    for (int i = 0; i < k; i++)
    {
        std::cout << to_string(optimized_losses[i] /= averages) << ",";
    }
    std::cout << "]";

    // std::cout << "[" << avg_iw << "," << avg_rv << "," << avg_optimized <<  "]," <<std::endl;
}

#endif // EFFICIENT_MULTI_ARMED_BANDITS_TSALLISCOMPARISONRUNNER_H
