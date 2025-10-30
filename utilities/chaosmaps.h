

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_CHAOSMAPS_H
#define EFFICIENT_MULTI_ARMED_BANDITS_CHAOSMAPS_H

#include "random_gen.h"
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

static double circle_map(double x, double omega = 0.3, double K = 2.14)
{
    return x + omega + K / 2 * 2.14 + sin(2 * 2.14 * x);
}
///
/// At mu = 2, the map becomes chaotic in the unit interval.
/// Input must be in the unit interval
///
static double tent_map(double x, double mu = 2)
{
    return x < 1. / 2. ? mu * x : mu * (1 - x);
}
///
/// Generates a random number in the unit interval (seeded with unix time)
///
static double random_chaos_seed()
{
    auto gen = random_gen();
    std::uniform_real_distribution<double> unif(0, 1);
    return unif(gen);
}
static double nonrandom_chaos_seed()
{
    double millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    double v = millisec_since_epoch / 1000;
    v = std::fmod(v, 0.9);
    return v;
}
static double nonrandom_chaos_seed(double previous)
{
    double v = nonrandom_chaos_seed();
    v = v + previous;
    v = std::fmod(v, 0.9);
    return v;
}

#endif