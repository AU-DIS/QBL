

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATA_GENERATORS_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATA_GENERATORS_H
#include "dataset.h"
#include "../utilities/chaosmaps.h"
#include "../experiments/DuelArena.h"
#include "iostream"
#include <boost/math/distributions/beta.hpp>
#include <boost/random/beta_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

std::vector<std::discrete_distribution<int>> create_distributions(int k)
{
    std::vector<std::discrete_distribution<int>> distributions(k);

    std::normal_distribution<double> gamma_distribution(1.2, 1);

    auto gen = random_gen();

    std::vector<double> probabilities(k);
    for (int i = 0; i < k; i++)
    {
        double p = gamma_distribution(gen);
        probabilities[i] = p;
    }

    double max_p = *std::max_element(probabilities.begin(), probabilities.end());
    double min_p = *std::min_element(probabilities.begin(), probabilities.end());

    for (int i = 0; i < k; i++)
    {
        probabilities[i] += min_p;
        probabilities[i] /= (max_p + min_p);
        std::vector<double> weights = {probabilities[i], 1. - probabilities[i]};
        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        distributions[i] = dist;
    }
    return distributions;
}

std::vector<boost::random::beta_distribution<>>
create_distributions(int k, int optimal_start, int optimal_end, double delta)
{
    //std::vector<std::discrete_distribution<int>> distributions(k);
    std::vector<boost::random::beta_distribution<>> distributions(k);
    auto gen = random_gen();

    //std::discrete_distribution<int> optimal_distribution({1 - delta, delta});
    // std::uniform_real_distribution<double> suboptimal_distribution;

    std::uniform_real_distribution<> uni(0, 1);
    boost::random::beta_distribution<> optimal_distribution(5,1);//(8, 1.1);
    boost::random::beta_distribution<> suboptimal_distribution(4, 20);

    //std::normal_distribution<double> suboptimal_distribution(1 - delta, 1);
    std::vector<double> probabilities(k);

    for (int i = 0; i < k; i++)
    {
        if (i >= optimal_start && i <= optimal_end)
        {
            //double p = quantile(optimal_distribution, uni(gen));
            //probabilities[i] = p;

            //Edited
            boost::random::beta_distribution<> optimal_distribution(5,1);//(8, 1.1);
            distributions[i] = optimal_distribution;
        }
        else
        {
            //double p = quantile(suboptimal_distribution, uni(gen));
                //std::cout << p << std::endl;
                //p = abs(p);
                //if (p >= 1) p = 0.1;
            //probabilities[i] = p;

            //edited
            boost::random::beta_distribution<> suboptimal_distribution(4,20);//(8, 1.1);
            distributions[i] = suboptimal_distribution;

        }
    }

    /*for (int i = 0; i < k; i++)
    {
        std::cout << probabilities[i] << " ";
        std::vector<double> weights = {1 - probabilities[i], probabilities[i]};
        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        distributions[i] = dist;
    }
    std::cout << std::endl;*/

    return distributions;
}

std::vector<std::vector<double>> stochastically_constrained_adversarial(int k, double delta, int rounds, double gap)
{
    auto gen = random_gen();

    std::discrete_distribution<int> first_optimal({0, 1});
    std::discrete_distribution<int> first_suboptimal({1 - delta, delta});

    std::discrete_distribution<int> second_optimal({1 - delta, delta});
    std::discrete_distribution<int> second_suboptimal({0, 1});

    auto arm_picker_weights = std::vector<int>(k, 1);
    std::discrete_distribution<int> optimal_arm_picker(arm_picker_weights.begin(), arm_picker_weights.end());

    std::vector<std::vector<double>> data_matrix;
    data_matrix.reserve(k);

    for (int i = 0; i < k; i++)
    {
        std::vector<double> row(rounds);
        data_matrix.push_back(row);
    }

    auto current_optimal = first_optimal;
    auto current_suboptimal = first_suboptimal;
    auto is_first = true;
    int multiple = 1;
    int optimal_arm = 1;
    for (int round = 0; round < rounds; round++)
    {
        double threshold = pow(gap, multiple);
        if (round > threshold)
        {
            optimal_arm = optimal_arm_picker(gen);
            current_optimal = is_first ? second_optimal : first_optimal;
            current_suboptimal = is_first ? second_suboptimal : second_optimal;
            is_first = !is_first;
            multiple++;
        }

        for (int arm = 0; arm < k; arm++)
        {
            data_matrix[arm][round] = current_suboptimal(gen);
        }
        data_matrix[optimal_arm][round] = current_optimal(gen);
    }
    return data_matrix;
}

class SinusDataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    std::mt19937 _gen;

public:
    SinusDataset() = default;
    SinusDataset(int k, int rounds)
    {
        _k = k;
        _rounds = rounds;
    }
    std::vector<std::vector<double>> generate()
    {
        // Fill the vector with uniformly distributed values
        std::vector<int> optimal_at_r(_rounds, 0);
        for (int i = 0; i < _rounds; i++)
        {
            optimal_at_r[i] = i % _k;
        }
        // Chosen arbitrarily
        int optimal_arm = 1;
        auto rng = random_gen();
        int number_of_optimal = _k * 2;
        std::shuffle(std::begin(optimal_at_r), std::end(optimal_at_r), rng);
        for (int i = 0; i < number_of_optimal; i++)
        {
            optimal_at_r[i] = optimal_arm;
        }
        std::shuffle(std::begin(optimal_at_r), std::end(optimal_at_r), rng);

        std::vector<std::vector<double>> data_matrix;
        for (int arm = 0; arm < _k; arm++)
        {
            auto r = std::vector<double>(_rounds, 0);
            for (int i = 0; i < _rounds; i++)
            {
                r[i] = optimal_at_r[i] == arm ? 1 : 0;
            }
            data_matrix.push_back(r);
        }
        return data_matrix;
    }
};

class StochasticDataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    double _lambda{};
    std::mt19937 _gen;

public:
    StochasticDataset() = default;
    StochasticDataset(int k, int rounds, double lambda)
    {
        _k = k;
        _rounds = rounds;
        _lambda = lambda;
        _gen = random_gen();
    }

    std::vector<std::vector<double>> generate()
    {
        std::vector<std::vector<double>> data_matrix;
        data_matrix.reserve(_k);

        for (int arm = 0; arm < _k; arm++)
        {
            double p = _lambda - (((double)arm) / _k);
            if (p < 0)
                p *= -1;
            std::discrete_distribution<int> bernoulli({1 - p, p});
            std::vector<double> row;
            row.reserve(_rounds);
            for (int round = 0; round < _rounds; round++)
            {
                double v = bernoulli(_gen);
                row.push_back(v);
            }
            data_matrix.push_back(row);
        }
        return data_matrix;
    }
};

class StochasticallyConstrainedDataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    double _gap{};
    double _delta{};

public:
    StochasticallyConstrainedDataset() = default;
    StochasticallyConstrainedDataset(int k, int rounds, double gap, double delta)
    {
        _k = k;
        _rounds = rounds;
        _gap = gap;
        _delta = delta;
    }

    std::vector<std::vector<double>> generate()
    {
        int k = _k;
        int rounds = _rounds;
        double gap = _gap;
        double delta = _delta;
        boost::mt19937 randGen(15);
        auto gen = random_gen();
        std::vector<std::vector<double>> data_matrix;
        data_matrix.reserve(k);

        std::uniform_int_distribution<> uni(0, _k-1);

        int first_end = std::max(1,_k/10);
        auto distributions = create_distributions(k, 0, first_end, delta);
        //auto distributions = create_distributions(k, 0, 2, delta);
        int old_start = 0;
        int old_top = first_end;
        for (int i = 0; i < k; i++)
        {
            std::vector<double> row(rounds, 0);
            data_matrix.push_back(row);
        }
        bool even = true;

        int multiple = 1;
        int multiple_mod = 1;
        for (int i = 0; i < rounds; i++)
        {
            double threshold = pow(gap, multiple);
            if (i > threshold)
            {
                if (first_end*multiple_mod >= _k) {
                    multiple_mod = 1;
                    distributions = create_distributions(k, old_start+1, first_end, delta);
                    old_start += 1;
                    old_top = first_end;
                } else {
                    distributions = create_distributions(k, old_top+1, old_top+first_end, delta);
                }
                old_top += first_end;//(old_top+3)%k;
                even = !even;
                multiple++;
                multiple_mod++;
            }

            auto one_triggered = false;
            for (int arm = 0; arm < k; arm++)
            {
                //boost::variate_generator<boost::mt19937, boost::random::uniform_real_distribution<> > generator(randGen, distributions[arm]);
                auto r = distributions[arm](gen);
                //if (!one_triggered && r == 1) one_triggered = true;
                data_matrix[arm][i] = r;
                //std::cout << r << " ";
            }
            if (!one_triggered) {
                /*for (int j = 0; j < k; j++) {
                    std::cout << data_matrix[j][i] << " ";
                }*/
                //std::cout << i << std::endl;
                //data_matrix[uni(gen)][i] = 1;
            }
            //data_matrix[i % k][i] += one_triggered ? 0 : 1;
        }

        return data_matrix;
    }
};

class BernoulliOptimalDataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    double _gap{};
    double _optimal_probability{};
    double _optimal_proportion{};
    double _delta{};

public:
    BernoulliOptimalDataset() = default;
    BernoulliOptimalDataset(int k, int rounds, double gap, double optimal_probability, double optimal_proportion, double delta)
    {
        _k = k;
        _rounds = rounds;
        _gap = gap;
        _optimal_probability = optimal_probability;
        _optimal_proportion = optimal_proportion;
        _delta = delta;
    }
    std::vector<std::vector<double>> generate()
    {
        int k = _k;
        int rounds = _rounds;
        double gap = _gap;
        std::vector<std::vector<double>> data_matrix;
        data_matrix.reserve(k);
        auto is_optimal = std::vector<bool>(_k, false);

        std::discrete_distribution<int> optimal({1 - _optimal_probability, _optimal_probability});
        std::discrete_distribution<int> suboptimal({1 - _optimal_probability - _delta, _optimal_probability - _delta});
        std::discrete_distribution<bool> optimal_picker({1 - _optimal_proportion, _optimal_proportion});

        auto gen = random_gen();
        for (int i = 0; i < _k; i++)
            is_optimal[i] = optimal_picker(gen);

        for (int i = 0; i < k; i++)
        {
            std::vector<double> row(rounds, 0);
            data_matrix.push_back(row);
        }

        int multiple = 1;
        for (int i = 0; i < rounds; i++)
        {
            double threshold = pow(gap, multiple);
            if (i > threshold)
            {
                for (int i = 0; i < _k; i++)
                    is_optimal[i] = optimal_picker(gen);
                multiple++;
            }
            auto one_triggered = false;
            for (int arm = 0; arm < k; arm++)
            {
                auto distribution = is_optimal[arm] ? optimal : suboptimal;
                auto r = distribution(gen);
                one_triggered = r == 1 ? true : false;
                data_matrix[arm][i] = r;
            }
            data_matrix[i % _k][i] += one_triggered ? 0 : 1;
        }
        return data_matrix;
    }
};

class Mod2Dataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    double _gap{};

public:
    Mod2Dataset() = default;
    Mod2Dataset(int k, int rounds, double gap)
    {
        _k = k;
        _rounds = rounds;
        _gap = gap;
    }
    std::vector<std::vector<double>> generate()
    {
        int k = _k;
        int rounds = _rounds;
        double gap = _gap;
        std::vector<std::vector<double>> data_matrix;
        data_matrix.reserve(k);
        for (int i = 0; i < k; i++)
        {
            std::vector<double> row(rounds, 0);
            data_matrix.push_back(row);
        }
        bool even = true;

        int multiple = 1;
        auto gen = random_gen();
        auto even_distribution = std::discrete_distribution<int>({0, 1});
        auto uneven_distribution = std::discrete_distribution<int>({1, 0});

        for (int i = 0; i < rounds; i++)
        {
            double threshold = pow(gap, multiple);
            if (i > threshold)
            {
                even = !even;
                multiple++;
            }
            for (int arm = 0; arm < k; arm++)
            {
                if (arm%2 == 0) {
                    data_matrix[arm][i] = even  ? even_distribution(gen) : uneven_distribution(gen);//arm % 2 : (arm + 1) % 2;
                } else {
                    data_matrix[arm][i] = even  ? uneven_distribution(gen) : even_distribution(gen);
                }
              }
        }
        return data_matrix;
    }
};

template <typename Bandit>
class DuellingDataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    Bandit _b{};

public:
    DuellingDataset() = default;

    DuellingDataset(Bandit &b, int k, int rounds)
    {
        _k = k;
        _rounds = rounds;
        _b = b;
    }

    std::vector<std::vector<double>> generate()
    {
        auto g = Bandit(_b);
        /*auto q = Bandit(_b);
        auto optimals = create_adversarial_dataset(q, g, _k, _rounds);
        */
        auto optimals = create_reflective_adversarial_dataset(g, _k, _rounds);
        std::vector<std::vector<double>> data_matrix;
        for (int arm = 0; arm < _k; arm++)
        {
            auto rewards = std::vector<double>(_rounds, 0);
            // std::cout << std::to_string(optimals[arm]) + "\n";
            data_matrix.push_back(rewards);
        }
        // Now loop over each optimals and set the arms reward to 1 in that round
        for (int i = 0; i < _rounds; i++)
        {
            int optimal = optimals[i];
            data_matrix[optimal][i] = 1;
        }

        return data_matrix;
    }
};

template <typename Bandit>
class DuellingDatasetTopk : public Dataset
{
private:
    int _k{};
    int _rounds{};
    int _m{};
    Bandit _b{};

public:
    DuellingDatasetTopk() = default;

    DuellingDatasetTopk(Bandit &b, int k, int rounds, int m)
    {
        _k = k;
        _rounds = rounds;
        _b = b;
        _m = m;

    }
    std::vector<std::vector<double>> generate()
    {
        auto g = Bandit(_b);
        auto optimals = create_reflective_adversarial_dataset_topk(g, _k, _rounds, _m);
        std::vector<std::vector<double>> data_matrix;
        for (int arm = 0; arm < _k; arm++)
        {
            auto rewards = std::vector<double>(_rounds, 0);
            // std::cout << std::to_string(optimals[arm]) + "\n";
            data_matrix.push_back(rewards);
        }
        for (int i = 0; i < _rounds; i++)
        {
            for (auto optimal : optimals[i])
            {
                data_matrix[optimal][i] = 1;
                //std::cout << std::to_string(optimal) + ", ";
            }
            //std::cout << std::endl;
        }
        return data_matrix;
    }
};

class TentMapDataset : public Dataset
{
private:
    int _k{};
    int _rounds{};
    double _gap{};

public:
    TentMapDataset() = default;
    TentMapDataset(int k, int rounds, double gap)
    {
        _k = k;
        _rounds = rounds;
        _gap = gap;
    }
    std::vector<std::vector<double>> generate()
    {
        int k = _k;
        int rounds = _rounds;
        double gap = _gap;
        std::vector<std::vector<double>> data_matrix;

        auto gen = random_gen();

        double v = nonrandom_chaos_seed();
        v = tent_map(v);
        int optimal_arm = (int)(v * _k);
        v = tent_map(v);
        auto suboptimal_distribution = std::discrete_distribution<int>({0.9, 0.1});
        auto optimal_distribution = std::discrete_distribution<int>({0.1, 0.9});

        for (int i = 0; i < k; i++)
        {
            std::vector<double> row(rounds, 0);
            data_matrix.push_back(row);
        }

        int multiple = 1;
        for (int i = 0; i < rounds; i++)
        {
            double threshold = pow(gap, multiple);
            if (i % 10 == 0)
                v = nonrandom_chaos_seed(v);

            if (i > threshold)
            {
                optimal_arm = (int)(v * _k);
                v = tent_map(v);
                multiple++;
            }
            auto one_triggered = false;
            for (int arm = 0; arm < k; arm++)
            {
                data_matrix[arm][i] = arm == optimal_arm ? optimal_distribution(gen) : (suboptimal_distribution(gen));
            }
        }
        return data_matrix;
    }
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_DATA_GENERATORS_H
