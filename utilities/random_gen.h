

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_RANDOM_GEN_H
#define EFFICIENT_MULTI_ARMED_BANDITS_RANDOM_GEN_H
#include "random"
#include <chrono>
static std::mt19937 random_gen()
{
    std::mt19937 random_gen{ static_cast<unsigned int>(
                                     std::chrono::steady_clock::now().time_since_epoch().count()
                             )};

    return random_gen;
}

#endif //EFFICIENT_MULTI_ARMED_BANDITS_RANDOM_GEN_H
