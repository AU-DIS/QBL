
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_UCB1_H
#define EFFICIENT_MULTI_ARMED_BANDITS_UCB1_H

template<typename bandit>
class UCB1 {

private:
    bandit _prototype;
    std::vector<bandit*> _bandits;
    std::vector<double> _accumulated_feedback;
    std::vector<int> _n_values;
    int _N = 0;
    int _last_played_bandit = 0;

public:
    ///
    /// \param K Number of bandit copies
    /// \param b Must have a copy constructor
    explicit UCB1(int K, bandit &b)
    :_prototype(b)
    {
        _prototype.choose();
        for (int i = 0; i < K; i++) {
            // This should call copy constructor
            auto b2 = new bandit(_prototype);
            // Must be called once initially
            int choice = b2->choose();
            _accumulated_feedback.push_back(1/K);
            _n_values.push_back(1);

            // Now store the bandit
            _bandits.push_back(b2);
            _N += 1;
        }
    }

    int choose() {
        double greatest_upper_bound = 0;
        int arg_max = 0;

        for (int i = 0; i < _bandits.size(); i++) {
            double upper_bound = (_accumulated_feedback[i]/_n_values[i]) + sqrt(2 * log(_N) / _n_values[i]);
            if (upper_bound >= greatest_upper_bound) {
                arg_max = i;
                greatest_upper_bound = upper_bound;
            }
        }
        _n_values[arg_max] += 1;
        _N += 1;
        _last_played_bandit = arg_max;
        int choice =  _bandits[arg_max]->choose();
        return choice;
    }
    void give_reward(size_t index, double feedback) {
        _accumulated_feedback[_last_played_bandit] += feedback;
        _bandits[_last_played_bandit]->give_reward(index, feedback);
    }

};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_UCB1_H
