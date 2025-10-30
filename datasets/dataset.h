//
// Created by kaspe on 19-04-2022.
//

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_DATASET_H
#define EFFICIENT_MULTI_ARMED_BANDITS_DATASET_H
// Base class
class Dataset {
public:
    virtual std::vector<std::vector<double>> generate()= 0;
};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_DATASET_H
