

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_THEORETICALBOUNDRUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_THEORETICALBOUNDRUNNER_H

#include "../utilities/eta_mean_std_writer.h"
#include "../algorithms/Exp3Bandit/Exp31.h"
#include "../algorithms/Exp3Bandit/Exp3.h"
#include "../algorithms/Exp3m/Exp3m.h"
#include "../algorithms/Exp3m/DepRoundALIASStrategy.h"
#include "../algorithms/FPL/FPL.h"
#include "../algorithms/FPL/QBL.h"
#include "../algorithms/Tsallis-INF/Tsallis_IW.h"
#include "../runner.h"
#include "math.h"

template <typename Bandit>
void run_theoretical_bound_experiment_Exp3Gamma_varying_k(Bandit &bandit, const std::string &out_path, int averages = 50) {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 8; i++) {
        int k = pow(2, i);
        int T = 10000;
        double g = T * 10;
        double gamma = min(1.0, sqrt(k * log(k) / ((exp(1) - 1) * g)));
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            bandit._k = k;
            bandit._gamma = gamma;
            Bandit exp3(bandit);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner_returning_reward(exp3, data, T, regrets);
            auto E_Exp3 = sum_of_range(regrets, 0, T);
            double local_GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > local_GMax) {
                    local_GMax = local_best_GMax;
                }
            }
            average_weak_regret += (local_GMax - E_Exp3) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise = 2.0 * sqrt(exp(1) - 1) * sqrt((double) g * (double) k * log((double) k));
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) T, average_weak_regret, (double) promise, average_divided_by_promise,
                           (double) averages, (double) g, (double) gamma};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3 (gamma) with varying k's"};
    vector<string> header{"k", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "g", "gamma"};
    write_results(data_matrix, comments, header,
                  out_path);
}

template <typename Bandit>
void run_theoretical_bound_experiment_Exp3Gamma_varying_T(Bandit &bandit, const std::string &out_path, int averages = 50) {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int k = 10;
        int T = pow(10, i);
        double g = T * 10;
        double gamma = min(1.0, sqrt(k * log(k) / ((exp(1) - 1) * g)));
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            bandit._k = k;
            bandit._gamma = gamma;
            Bandit exp3(bandit);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner_returning_reward(exp3, data, T, regrets);
            auto EExp3 = sum_of_range(regrets, 0, T);
            double local_GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > local_GMax) {
                    local_GMax = local_best_GMax;
                }
            }
            average_weak_regret += (local_GMax - EExp3) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise = 2.0 * sqrt(exp(1) - 1) * sqrt((double) g * (double) k * log((double) k));
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) T, average_weak_regret, (double) promise, average_divided_by_promise,
                           (double) averages, (double) g, (double) gamma};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3 (gamma) with varying T's"};
    vector<string> header{"k", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "g", "gamma"};
    write_results(data_matrix, comments, header,
                  out_path);
}

void run_theoretical_bound_experiment_Exp31_varying_k(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 8; i++) {
        int k = pow(2, i);
        int T = 10000;
        double average_strong_regret = 0;
        double promise = 0;
        auto F1 = [](double K) { return 10.5 * sqrt(K * log(K)); };
        auto F2 = [](double K) { return 13.8 + 2 * K * log(K); };
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            Exp31 exp31(k);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner(exp31, data, T, regrets);
            auto sum_regret = sum_of_range(regrets, 0, T);
            double GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > GMax) {
                    GMax = local_best_GMax;
                }
            }
            double best_multichoice_reward = 0;
            for (int round = 0; round < T; round++) {
                double round_best_multichoice_reward = 0;
                for (int arm = 0; arm < k; arm++) {
                    if (data[arm][round] > round_best_multichoice_reward) {
                        round_best_multichoice_reward = data[arm][round];
                    }
                }
                best_multichoice_reward += round_best_multichoice_reward;
            }
            average_strong_regret += sum_regret / (double) averages;
            double Lopt = best_multichoice_reward - GMax;
            promise += Lopt + F1(k) * sqrt(T) + F2(k) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double average_divided_by_promise = average_strong_regret / promise;
        vector<double> run{(double) k, (double) T, average_strong_regret, (double) promise, average_divided_by_promise,
                           (double) averages};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3.1 with varying k's"};
    vector<string> header{"k", "T", "average_strong_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp31_with_varying_k.csv");
}

void run_theoretical_bound_experiment_Exp31_varying_T(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 9; i++) {
        int k = 3;
        int T = pow(10, i);
        double average_strong_regret = 0;
        double promise = 0;
        auto F1 = [](double K) { return 10.5 * sqrt(K * log(K)); };
        auto F2 = [](double K) { return 13.8 + 2 * K * log(K); };
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            Exp31 exp31(k);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner(exp31, data, T, regrets);
            auto sum_regret = sum_of_range(regrets, 0, T);
            double GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > GMax) {
                    GMax = local_best_GMax;
                }
            }
            double best_multichoice_reward = 0;
            for (int round = 0; round < T; round++) {
                double round_best_multichoice_reward = 0;
                for (int arm = 0; arm < k; arm++) {
                    if (data[arm][round] > round_best_multichoice_reward) {
                        round_best_multichoice_reward = data[arm][round];
                    }
                }
                best_multichoice_reward += round_best_multichoice_reward;
            }
            average_strong_regret += sum_regret / (double) averages;
            double Lopt = best_multichoice_reward - GMax;
            promise += Lopt + F1(k) * sqrt(T) + F2(k) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double average_divided_by_promise = average_strong_regret / promise;
        vector<double> run{(double) k, (double) T, average_strong_regret, (double) promise, average_divided_by_promise,
                           (double) averages};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3.1 with varying T's"};
    vector<string> header{"k", "T", "average_strong_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp31_with_varying_T.csv");
}

void run_theoretical_bound_experiment_Exp3ix_varying_k(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 10; i++) {
        int k = pow(2, i);
        int T = 1000000;
        double average = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            double eta = sqrt(log(k) / ((double) T * (double) k));
            double gamma = eta / 2;
            Exp3IXTor exp3IX(k, eta, gamma);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner(exp3IX, data, T, regrets);
            auto Rn = sum_of_range(regrets, 0, T);
            average += Rn / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double average_divided_by_promise = average / sqrt((double) T * (double) k * log((double) k));
        vector<double> run{(double) k, (double) T, average, average_divided_by_promise, (double) averages};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3-IX with varying K's"};
    vector<string> header{"k", "T", "average", "average_divided_by_promise", "number_of_average_runs"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp3ix_with_varying_k.csv");
}

void run_theoretical_bound_experiment_Exp3ix_varying_T(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 10; i++) {
        int k = 10000;
        int T = pow(10, i);
        double average = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            double eta = sqrt(log(k) / ((double) T * (double) k));
            double gamma = eta / 2;
            Exp3IXTor exp3IX(k, eta, gamma);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner(exp3IX, data, T, regrets);
            auto Rn = sum_of_range(regrets, 0, T);
            average += Rn / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double average_divided_by_promise = average / sqrt((double) T * (double) k * log((double) k));
        vector<double> run{(double) k, (double) T, average, average_divided_by_promise, (double) averages};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3-IX with varying T's"};
    vector<string> header{"k", "T", "average", "average_divided_by_promise", "number_of_average_runs"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp3ix_with_varying_T.csv");
}

template <typename Bandit>
void run_theoretical_bound_experiment_FPL_varying_k(Bandit &bandit, const std::string &out_path, int averages = 50) {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int k = pow(2, i);
        int T = 100;
        // We have not found the actual eta for the theoretical bounds, but we have found 10 to work for other experiments
        double eta = 10;
        double average_strong_regret = 0;
        double R = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            bandit._k = k;
            bandit._eta = eta;
            bandit._number_to_presample = T;
            Bandit fpl(bandit);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner(fpl, data, T, regrets);
            auto Rn = sum_of_range(regrets, 0, T);
            average_strong_regret += Rn / (double) averages;
            double GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > GMax) {
                    GMax = local_best_GMax;
                }
            }
            double best_multichoice_reward = 0;
            for (int round = 0; round < T; round++) {
                double round_best_multichoice_reward = 0;
                for (int arm = 0; arm < k; arm++) {
                    if (data[arm][round] > round_best_multichoice_reward) {
                        round_best_multichoice_reward = data[arm][round];
                    }
                }
                best_multichoice_reward += round_best_multichoice_reward;
            }
            double worst_multichoice_reward = 0;
            for (int round = 0; round < T; round++) {
                double round_worst_multichoice_reward = 1;
                for (int arm = 0; arm < k; arm++) {
                    if (data[arm][round] < round_worst_multichoice_reward) {
                        round_worst_multichoice_reward = data[arm][round];
                    }
                }
                worst_multichoice_reward += round_worst_multichoice_reward;
            }
            R += (best_multichoice_reward - worst_multichoice_reward) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise = R * pow(k, 5.0 / 3.0) * pow(log((double) k), 1.0 / 3.0) * pow(T, 2.0 / 3.0);
        double average_divided_by_promise = average_strong_regret / promise;
        vector<double> run{(double) k, (double) T, average_strong_regret, promise, average_divided_by_promise,
                           (double) averages, eta};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for FPL with varying K's"};
    vector<string> header{"k", "T", "average_strong_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "eta"};
    write_results(data_matrix, comments, header, out_path);
}

template <typename Bandit>
void run_theoretical_bound_experiment_FPL_varying_T(Bandit &bandit, const std::string &out_path, int averages = 50) {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int k = 3;
        int T = pow(10, i);
        // We have not found the actual eta for the theoretical bounds, but we have found 10 to work for other experiments
        double eta = 10;
        double average_strong_regret = 0;
        double R = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            bandit._k = k;
            bandit._eta = eta;
            bandit._number_to_presample = T;
            Bandit fpl(bandit);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            basic_runner(fpl, data, T, regrets);
            auto Rn = sum_of_range(regrets, 0, T);
            average_strong_regret += Rn / (double) averages;
            double GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > GMax) {
                    GMax = local_best_GMax;
                }
            }
            double best_multichoice_reward = 0;
            for (int round = 0; round < T; round++) {
                double round_best_multichoice_reward = 0;
                for (int arm = 0; arm < k; arm++) {
                    if (data[arm][round] > round_best_multichoice_reward) {
                        round_best_multichoice_reward = data[arm][round];
                    }
                }
                best_multichoice_reward += round_best_multichoice_reward;
            }
            double worst_multichoice_reward = 0;
            for (int round = 0; round < T; round++) {
                double round_worst_multichoice_reward = 1;
                for (int arm = 0; arm < k; arm++) {
                    if (data[arm][round] < round_worst_multichoice_reward) {
                        round_worst_multichoice_reward = data[arm][round];
                    }
                }
                worst_multichoice_reward += round_worst_multichoice_reward;
            }
            R += (best_multichoice_reward - worst_multichoice_reward) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise = R * pow(k, 5.0 / 3.0) * pow(log((double) k), 1.0 / 3.0) * pow(T, 2.0 / 3.0);
        double average_divided_by_promise = average_strong_regret / promise;
        vector<double> run{(double) k, (double) T, average_strong_regret, promise, average_divided_by_promise,
                           (double) averages, eta};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for FPL with varying T's"};
    vector<string> header{"k", "T", "average_strong_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "eta"};
    write_results(data_matrix, comments, header, out_path);
}

void run_theoretical_bound_experiment_Exp3m_varying_m(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int m = pow(2, i);
        int k = 100;
        int T = 100000;
        double gamma = min(1.0, sqrt(k * log(k / m) / ((exp(1) - 1) * m * T)));
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            Exp3m exp3m(m, k, gamma);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            top_k_runner_returning_reward(exp3m, data, T, k, m, regrets);
            auto GExp3m = sum_of_range(regrets, 0, T);
            std::vector<double> sum_of_each_arm;
            sum_of_each_arm.reserve(k);
            for (int i = 0; i < k; i++) {
                sum_of_each_arm.push_back(sum_of_range(data[i], 0, T));
            }
            auto sorted_best_arms = argsort(sum_of_each_arm);
            double GMax_m = 0;
            for (int i = k - m - 1; i < k; i++) {
                GMax_m += sum_of_each_arm[sorted_best_arms[i]] / (double) averages;
            }
            average_weak_regret += (GMax_m - GExp3m) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise =
                2 * sqrt(exp(1) - 1) * sqrt((double) m * (double) T * (double) k * log((double) k / (double) m));
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) m, (double) T, average_weak_regret, promise,
                           average_divided_by_promise, (double) averages, gamma};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3.M with varying m's"};
    vector<string> header{"k", "m", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "gamma"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp3M_with_varying_m.csv");
}

void run_theoretical_bound_experiment_Exp3m_varying_k(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int m = 10;
        int k = 10 * pow(2, i);
        int T = 100000;
        double gamma = min(1.0, sqrt(k * log(k / m) / ((exp(1) - 1) * m * T)));
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            Exp3m exp3m(m, k, gamma);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            top_k_runner_returning_reward(exp3m, data, T, k, m, regrets);
            auto GExp3m = sum_of_range(regrets, 0, T);
            std::vector<double> sum_of_each_arm;
            sum_of_each_arm.reserve(k);
            for (int i = 0; i < k; i++) {
                sum_of_each_arm.push_back(sum_of_range(data[i], 0, T));
            }
            auto sorted_best_arms = argsort(sum_of_each_arm);
            double GMax_m = 0;
            for (int i = k - m - 1; i < k; i++) {
                GMax_m += sum_of_each_arm[sorted_best_arms[i]] / (double) averages;
            }
            average_weak_regret += (GMax_m - GExp3m) / (double) averages;
        }
        // The divided part is what we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise =
                2 * sqrt(exp(1) - 1) * sqrt((double) m * (double) T * (double) k * log((double) k / (double) m));
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) m, (double) T, average_weak_regret, promise,
                           average_divided_by_promise, (double) averages, gamma};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3.M with varying k's"};
    vector<string> header{"k", "m", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "gamma"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp3M_with_varying_k.csv");
}

void run_theoretical_bound_experiment_Exp3m_varying_T(int averages = 50, const std::string &out_path = "/tmp") {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int m = 10;
        int k = 100;
        int T = pow(10, i);
        double gamma = min(1.0, sqrt(k * log(k / m) / ((exp(1) - 1) * m * T)));
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            Exp3m exp3m(m, k, gamma);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> regrets;
            top_k_runner_returning_reward(exp3m, data, T, k, m, regrets);
            auto GExp3m = sum_of_range(regrets, 0, T);
            std::vector<double> sum_of_each_arm;
            sum_of_each_arm.reserve(k);
            for (int i = 0; i < k; i++) {
                sum_of_each_arm.push_back(sum_of_range(data[i], 0, T));
            }
            auto sorted_best_arms = argsort(sum_of_each_arm);
            double GMax_m = 0;
            for (int i = k - m - 1; i < k; i++) {
                GMax_m += sum_of_each_arm[sorted_best_arms[i]] / (double) averages;
            }
            average_weak_regret += (GMax_m - GExp3m) / (double) averages;
        }
        // The divided part is what that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise =
                2 * sqrt(exp(1) - 1) * sqrt((double) m * (double) T * (double) k * log((double) k / (double) m));
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) m, (double) T, average_weak_regret, promise,
                           average_divided_by_promise, (double) averages, gamma};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Exp3.M with varying T's"};
    vector<string> header{"k", "m", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs", "gamma"};
    write_results(data_matrix, comments, header, out_path + "/Theoretical_Bound_Experiment_Exp3M_with_varying_T.csv");
}

template <typename Bandit>
void run_theoretical_bound_experiment_Tsallis_varying_k(Bandit &bandit, const std::string &out_path, int averages = 50) {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 8; i++) {
        int k = pow(2, i);
        int T = 10000;
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            bandit._k = k;
            Bandit tsallis(bandit);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> reward;
            basic_runner_returning_reward(tsallis, data, T, reward);
            double local_GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > local_GMax) {
                    local_GMax = local_best_GMax;
                }
            }
            average_weak_regret += (local_GMax - sum_of_range(reward, 0, T)) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise = 4 * sqrt((double) k * (double) T) + 1;
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) T, average_weak_regret, (double) promise, average_divided_by_promise,
                           (double) averages};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Tsallis IW with varying k's"};
    vector<string> header{"k", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs"};
    write_results(data_matrix, comments, header,out_path);
}

template <typename Bandit>
void run_theoretical_bound_experiment_Tsallis_varying_T(Bandit &bandit, const std::string &out_path, int averages = 50) {
    vector<vector<double>> data_matrix;
    for (int i = 1; i < 7; i++) {
        int k = 10;
        int T = pow(10, i);
        double average_weak_regret = 0;
        StochasticallyConstrainedDataset d(k, T, 3.2, 0.9);
        for (int run = 0; run < averages; run++) {
            bandit._k = k;
            Bandit tsallis(bandit);
            std::vector<std::vector<double>> data = d.generate();
            std::vector<double> reward;
            basic_runner_returning_reward(tsallis, data, T, reward);
            double local_GMax = 0;
            for (int arm = 0; arm < k; arm++) {
                double local_best_GMax = 0;
                for (int round = 0; round < T; round++) {
                    local_best_GMax += data[arm][round];
                }
                if (local_best_GMax > local_GMax) {
                    local_GMax = local_best_GMax;
                }
            }
            average_weak_regret += (local_GMax - sum_of_range(reward, 0, T)) / (double) averages;
        }
        // The divided part is the O-time that we are promised so we would expect these different averages to be the
        // same i.e. a flat line when plotted
        double promise = 4 * sqrt((double) k * (double) T) + 1;
        double average_divided_by_promise = average_weak_regret / promise;
        vector<double> run{(double) k, (double) T, average_weak_regret, (double) promise, average_divided_by_promise,
                           (double) averages};
        data_matrix.push_back(run);
    }
    vector<string> comments{"#Theoretical Bound Experiment for Tsallis IW with varying k's"};
    vector<string> header{"k", "T", "average_weak_regret", "promise", "average_divided_by_promise",
                          "number_of_average_runs"};
    write_results(data_matrix, comments, header,out_path);
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_THEORETICALBOUNDRUNNER_H
