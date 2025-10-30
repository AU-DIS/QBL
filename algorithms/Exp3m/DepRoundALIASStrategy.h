

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DEPROUNDALIASSTRATEGY_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DEPROUNDALIASSTRATEGY_H

#include <vector>

class DepRoundALIASStrategy {
public:
    std::vector<int> dependent_weighted_choices(std::vector<double> probabilities, int k);
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_DEPROUNDALIASSTRATEGY_H
