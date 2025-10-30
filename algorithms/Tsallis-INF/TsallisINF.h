

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_TSALLISINF_H
#define EFFICIENT_MULTI_ARMED_BANDITS_TSALLISINF_H

#include <vector>
#include "tgmath.h"
#include <random>
#include "iostream"
#include "../../utilities/random_gen.h"


template<typename Estimator>
class TsallisINF {
private:
    int _t = 1;
    double _eta;
    std::mt19937 rg = random_gen();
    double _x = rg();


public:
    std::vector<double> _weights;
    std::vector<double> cumulative_losses;
    Estimator _estimator;
    double _k;

    TsallisINF(int k, Estimator &estimator) {
        _k = k;
        _estimator = estimator;
        cumulative_losses.reserve(k);
        for (int i = 0; i < _k; i++) cumulative_losses.push_back(0);
    }

    TsallisINF(const TsallisINF &prototype) {
        _k = prototype._k;
        _estimator = prototype._estimator;
        cumulative_losses.reserve(prototype.k);
        for (int i = 0; i < _k; i++) cumulative_losses.push_back(0);
    }

    double compute_eta(int t) {
        _eta = 1 / sqrt(std::max(1, t));
        return _eta;
    }

    /// This assumes alpha = 1/2
    std::vector<double> newtons_method_weights(std::vector<double> &losses, double eta) {

        std::vector<double> weights;
        double x_previous = _x;
        double x_estimated = _x;
        weights.reserve(losses.size());


        for (int i = 0; i < losses.size(); i++) weights.push_back(0);

        do {
            x_previous = x_estimated;
            for (int i = 0; i < losses.size(); i++) weights[i] = 4 * pow((eta * (losses[i] - x_previous)), -2);
            double w_sum_powered = 0;
            double w_sum = 0;
            for (auto w: weights) {
                w_sum_powered += pow(w, (3 / 2));
                w_sum += w;
            }
            x_estimated = x_previous - (w_sum - 1) / (eta * w_sum_powered);
        } while (std::min(x_previous, x_estimated) / std::max(x_previous, x_estimated) >= 1.1);
        _x = x_estimated;
        return weights;
    }

    int choose() {
        _weights = newtons_method_weights(cumulative_losses, compute_eta(_t));
        std::discrete_distribution<> d(_weights.begin(), _weights.end());

        int s = d(rg);
        _t += 1;
        return s;
    }

    void give_reward(size_t index, double feedback) {
        // We can either use IW or RV to construct the estimated reward
        // This is RW, we should also try RV, which is a reduced variance estimator
        auto estimators = _estimator.estimate(index, feedback, _weights, _eta);
        for (int i = 0; i < cumulative_losses.size(); i++) {
            cumulative_losses[i] += estimators[i];
        }
    }
};


#endif //EFFICIENT_MULTI_ARMED_BANDITS_TSALLISINF_H
