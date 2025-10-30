

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DUELARENA_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DUELARENA_H

#include "iostream"
#include "vector"
#include "../utilities/argsort.h"
#include "set"

template <typename Adversary, typename Bandit>
std::vector<int> run_bandit_duel(Adversary &q, Bandit &g, int k, int rounds)
{
    std::vector<double> q_regrets(rounds, 0);
    std::vector<double> g_regrets(rounds, 0);
    std::vector<int> choices(rounds, 0);

    for (int round = 0; round < rounds; round++)
    {
        int correct_choice = q.choose();
        int guess = g.choose();

        double adversary_reward = correct_choice == guess ? 0 : 1;
        double bandit_reward = correct_choice == guess ? 1 : 0;

        q.give_reward(correct_choice, adversary_reward);
        g.give_reward(guess, bandit_reward);
        q_regrets[round] = bandit_reward;
        g_regrets[round] = adversary_reward;
        // std::cout << std::to_string(correct_choice) + "\n";
        choices[round] = guess = correct_choice ? (correct_choice + 1) % k : correct_choice;
    }
    double q_regret = 0;
    double g_regret = 0;
    for (int i = 0; i < rounds; i++)
    {
        q_regret += q_regrets[i];
        g_regret += g_regrets[i];
    }
    // q_regret /= rounds;
    // g_regret /= rounds;
    // std::cout << "Adversary regret: " + std::to_string(q_regret) + " Bandit regret: " + std::to_string(g_regret) + "\n";
    return choices;
}

template <typename Bandit>
std::vector<std::vector<int>> run_reflective_duel_topk(Bandit bandit, int k, int rounds, int m)
{
    auto w = bandit.get_weights();
    std::vector<std::vector<int>> choices(rounds);
    for (int i = 0; i < rounds; i++)
    {
        auto C = bandit.choose(m);
        auto w_argsorted = argsort(*w);
        // Pick the choice the algorithm favours the least, or the second least if it picked it by random chance
        std::vector<int> adversarial_C;
        for (int j = 0; j < m; j++)
        {
            adversarial_C.push_back(w_argsorted[j]);
        }
        choices[i] = adversarial_C;
        std::vector<double> rewards;
        std::set<int> lowest_arms;
        for (int j = 0; j < m; j++)
        {
            lowest_arms.insert(w_argsorted[j]);
        }
        for (int j = 0; j < m; j++)
        {
            int c = C[j];
            double reward = lowest_arms.contains(c) ? 1. : 0.;
            rewards.push_back(reward);
        }

        bandit.give_reward(C, rewards);
    }
    return choices;
}

template <typename Bandit>
std::vector<int> run_reflective_duel(Bandit &bandit, int k, int rounds)
{
    auto w = bandit.get_weights();
    std::vector<int> choices(rounds, 0);
    for (int i = 0; i < rounds; i++)
    {
        int c = bandit.choose();
        auto w_argsorted = argsort(*w);
        // Pick the choice the algorithm favours the least, or the second least if it picked it by random chance
        int adversarial_c = w_argsorted[0];
        choices[i] = adversarial_c;
        bandit.give_reward(c, c == w_argsorted[0] ? 1 : 0);
    }
    return choices;
}

template <typename Bandit>
std::vector<std::vector<int>> create_reflective_adversarial_dataset_topk(Bandit &b, int k, int rounds, int m)
{

    double max_regret = 0;
    auto best_choices = std::vector<std::vector<int>>(rounds);

    for (int i = 0; i < 10; i++)
    {
        // Duelling phase
        auto bandit = Bandit(b);
        auto proposed_choices = run_reflective_duel_topk(bandit, k, rounds, m);

        // Simulation phase
        bandit = Bandit(b);
        double regret = 0;
        for (int r = 0; r < rounds; r++)
        {
            auto C = bandit.choose(m);
            std::set<int> proposed_set;
            for (auto c : proposed_choices[r])
                proposed_set.insert(c);

            auto reg = std::vector<double>();
            auto rew = std::vector<double>();
            for (auto c : C)
            {
                reg.push_back(proposed_set.find(c) != proposed_set.end() ? 1 : 0);
                rew.push_back(proposed_set.find(c) != proposed_set.end() ? 0 : 1);
                regret += proposed_set.find(c) != proposed_set.end() ? 1 : 0;
            }
            bandit.give_reward(C, rew);
        }
        if (regret > max_regret)
        {
            best_choices = proposed_choices;
            max_regret = regret;
        }
    }
    return best_choices;
}

template <typename Bandit>
std::vector<int> create_reflective_adversarial_dataset(Bandit &b, int k, int rounds)
{

    double max_regret = 0;
    auto best_choices = std::vector<int>(rounds, 0);

    for (int i = 0; i < 10; i++)
    {
        // Duelling phase
        auto bandit = Bandit(b);
        auto proposed_choices = run_reflective_duel(bandit, k, rounds);
        // Simulation phase
        bandit = Bandit(b);
        double regret = 0;
        for (int r = 0; r < rounds; r++)
        {
            int c = bandit.choose();
            regret += c == proposed_choices[r] ? 0 : 1;
            bandit.give_reward(c, c == proposed_choices[r] ? 1 : 0);
        }
        if (regret > max_regret)
        {
            best_choices = proposed_choices;
            max_regret = regret;
        }
    }
    return best_choices;
}

template <typename Adversary, typename Bandit>
std::vector<int> create_adversarial_dataset(Adversary &q, Bandit &g, int k, int rounds)
{
    double max_regret = 0;
    std::vector<int> worst_picks(k, 0);

    for (int i = 0; i < 10; i++)
    {

        double regret = 0;
        auto r_bandit = Bandit(g);
        // Duel phase
        auto proposed_dataset = run_bandit_duel(q, r_bandit, k, rounds);
        r_bandit = Bandit(g);
        // Simulation phase
        for (int r = 0; r < rounds; r++)
        {
            auto c = r_bandit.choose();
            auto reward = c == proposed_dataset[r] ? 1 : 0;
            regret += 1 - reward;
            r_bandit.give_reward(c, reward);
            // If bandit guesses correctly, then we switch out the choice
            if (c == proposed_dataset[r])
            {
                proposed_dataset[r] = (c + 1) % k;
            }
        }
        if (regret > max_regret)
        {
            worst_picks = proposed_dataset;
            max_regret = regret;
        }
    }
    return worst_picks;
}

#endif