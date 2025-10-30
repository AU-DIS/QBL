

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_RUNNER_H
#define EFFICIENT_MULTI_ARMED_BANDITS_RUNNER_H
#include "vector"
#include "set"
#include <iostream>

template <typename Bandit>
void basic_runner(Bandit &bandit, std::vector<std::vector<double>> &data_matrix, int rounds, std::vector<double> &regrets){
    regrets.reserve(rounds);
    for (int round = 0; round < rounds; round++) {
        auto choice = bandit.choose();
        auto reward = data_matrix[choice][round];
        double max_element = 0;
        for (auto & arm : data_matrix) {
            if (arm[round] >= max_element) max_element = arm[round];
        }
        double regret = max_element - reward;
        bandit.give_reward(choice, reward);
        regrets.push_back(regret);
    }
}


template <typename Bandit>
void basic_runner_returning_reward(Bandit &bandit, std::vector<std::vector<double>> &data_matrix, int rounds, std::vector<double> &regrets){
    regrets.reserve(rounds);
    for (int round = 0; round < rounds; round++) {
        auto choice = bandit.choose();
        auto reward = data_matrix[choice][round];
        double max_element = 0;
        for (auto & arm : data_matrix) {
            if (arm[round] >= max_element) max_element = arm[round];
        }
        double regret = max_element - reward;
        bandit.give_reward(choice, reward);
        regrets.push_back(reward);
    }
}

template <typename Bandit>
void single_top_k_runner(Bandit &bandit, std::vector<std::vector<double>> &data_matrix, int rounds, int k, int m, std::vector<double> &regrets){
    regrets.reserve(rounds);
    for (int round = 0; round < rounds; round++) {

        double round_regret = 0;

        std::set<int> k_set;
        while (k_set.size() < m) {
            int choice = bandit.choose();
            k_set.insert(choice);
        }
        std::set<int> taken;

        double max_regret = 0;
        double round_reward = 0;
        double max_choice = 0;

        vector<double> vec = vector<double>(k);
        for (int i = 0; i < k; i++) {
            vec[i] = data_matrix[i][round];
        }
        sort(vec.begin(), vec.end(), greater<>());
        for (int i = 0; i < m; i++) {
            max_choice += vec[i];
        }

        for (auto v : k_set) {
            auto reward = data_matrix[v][round];
            round_reward += reward;
            bandit.give_reward(v, reward);
        }

        round_regret = max_choice - round_reward;
        regrets.push_back(round_regret);
    }
}

template <typename Bandit>
void top_k_runner(Bandit &bandit, std::vector<std::vector<double>> &data_matrix, int rounds, int k, int m, std::vector<double> &regrets){

    regrets.reserve(rounds);
    for (int round = 0; round < rounds; round++) {
        double round_regret = 0;
        double max_regret = 0;
        double round_reward = 0;
        double max_choice = 0;

        vector<double> vec = vector<double>(k);
        for (int i = 0; i < k; i++) {
            vec[i] = data_matrix[i][round];
        }
        sort(vec.begin(), vec.end(), greater<>());
        for (int i = 0; i < m; i++) {
            max_choice += vec[i];
        }

        auto choices = bandit.choose(m);
        auto rewards = std::vector<double>();
        for (auto v : choices) {
            auto reward = data_matrix[v][round];
            rewards.push_back(reward);
            round_reward += reward;
        }
        round_regret = max_choice - round_reward;
        bandit.give_reward(choices, rewards);
        regrets.push_back(round_regret);
    }
}



template <typename Bandit>
void top_k_runner_returning_reward(Bandit &bandit, std::vector<std::vector<double>> &data_matrix, int rounds, int k, int m, std::vector<double> &regrets){

    regrets.reserve(rounds);
    for (int round = 0; round < rounds; round++) {
        double round_regret = 0;
        double max_regret = 0;
        double round_reward = 0;
        double max_choice = 0;

        vector<double> vec = vector<double>(k);
        for (int i = 0; i < k; i++) {
            vec[i] = data_matrix[i][round];
        }
        sort(vec.begin(), vec.end(), greater<>());
        for (int i = 0; i < m; i++) {
            max_choice += vec[i];
        }

        auto choices = bandit.choose(m);
        auto rewards = std::vector<double>();
        for (auto v : choices) {
            auto reward = data_matrix[v][round];
            rewards.push_back(reward);
            round_reward += reward;
        }
        bandit.give_reward(choices, rewards);
        regrets.push_back(round_reward);
    }
}


template <typename Bandit>
void exp3m_runner(Bandit &bandit, std::vector<std::vector<double>> &data_matrix, int rounds, int k, int m, std::vector<double> &regrets) {

    regrets.reserve(rounds);
    for (int round = 0; round < rounds; round++) {
        std::vector<double> uniform_rewards(k, 0);
        std::vector<double> exp3m_rewards(k, 0);
        std::vector<double> round_uniform_regrets(k, 0);
        std::vector<double> round_exp3m_regrets(k, 0);


        auto choices = bandit.choose(m);
        double uniform_regret = 0;
        double uniform_reward = 0;
        double exp3m_regret = 0;
        double exp3m_reward = 0;
        double max_choice = 0;
        vector<double> vec = vector<double>(k);
        for (int i = 0; i < k; i++) {
            vec[i] = data_matrix[i][round];
        }
        sort(vec.begin(), vec.end(), greater<>());
        for (int i = 0; i < m; i++) {
            max_choice += vec[i];
        }
        for (int c = 0; c < m; c++) {
            auto exp3m_choice = choices[c];
            exp3m_reward += data_matrix[exp3m_choice][round];
            exp3m_rewards[exp3m_choice] = data_matrix[exp3m_choice][round];
        }
        bandit.give_reward(choices, exp3m_rewards);
        exp3m_regret = ((max_choice) - exp3m_reward);
        regrets[round] = exp3m_regret;

    }
}



#endif //EFFICIENT_MULTI_ARMED_BANDITS_RUNNER_H
