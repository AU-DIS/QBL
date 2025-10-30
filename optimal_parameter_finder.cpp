

#include "algorithms/FPL/FPL.h"
#include "algorithms/Exp3Bandit/Exp3.h"
#include "datasets/data_generators.h"
#include "iostream"

template <typename Bandit>
double run_with_parameter(Bandit &bandit, std::vector<std::vector<double>> &dataset, int rounds)
{
    double acc_regret = 0;
    for (int round = 0; round < rounds; round++)
    {
        auto choice = bandit.choose();
        auto reward = dataset[choice][round];
        acc_regret += 1 - reward;
        bandit.give_reward(choice, reward);
    }
    return acc_regret;
}

template <typename Dataset, typename Bandit>
double find_optimal_parameter(Dataset &d, Bandit &prototype, double initial_parameter, int number_of_iterations, int number_of_rounds)
{
    double parameter = initial_parameter;
    double best_parameter = parameter;

    double r = number_of_rounds;

    while (parameter > 1 * 1.02)
    {
        parameter = parameter * 0.98;
        auto dataset = d.generate();
        auto b = Bandit(prototype);
        //b.set_parameter(parameter);
        double r1 = run_with_parameter(b, dataset, number_of_rounds);
        r1 /= number_of_rounds;
        if (r1 < r)
        {
            best_parameter = parameter;
            r = r1;
        }
    }
    return best_parameter;
}

int main(int argc, char *argv[])
{
    double sum_best = 0;
    auto ks = {8,
               16,
               32,
               64,
               128};
    int rounds = 1000;
    double gap = 3.2;
    double delta = 0.9;

    for (int i = 0; i < 10; i++)
    {
        for (auto k : ks)
        {
            auto b = FPL(k, 10);
            // auto b = Exp3(k, 0.1);
            StochasticallyConstrainedDataset sCD(k, rounds, gap, delta);
            Mod2Dataset mod2(k, rounds, gap);
            auto best1 = find_optimal_parameter(sCD, b, 100, 1000, rounds);
            auto best2 = find_optimal_parameter(mod2, b, 100, 1000, rounds);
            sum_best += (best1 + best2) / 2;
        }
        sum_best /= ks.size();
    }

    std::cout << std::to_string(sum_best / 10);
}