

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DEBUG_SIMPLE_STOCHASTIC_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DEBUG_SIMPLE_STOCHASTIC_H
#include <vector>

class debug_simple_stochastic {
public:
        explicit debug_simple_stochastic(int K);
        double feedback(int choice, double &regret);

        int k = 0;
        double mean_value;

    private:
        std::vector<double> _data_matrix;
    };

#endif //EFFICIENT_MULTI_ARMED_BANDITS_DEBUG_SIMPLE_STOCHASTIC_H
