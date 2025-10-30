
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_TSALLIS_LTU_H
#define EFFICIENT_MULTI_ARMED_BANDITS_TSALLIS_LTU_H

class Tsallis_LTU
{
private:
    int _t;
    double _eta;
    std::mt19937 _rg;
    double _x;
    std::vector<double> _weights;
    std::vector<double> _cumulative_losses;
    int _k;
    std::discrete_distribution<> _d;
    bool update_triggered = true;

    double compute_eta(int t)
    {
        _eta = 1 / sqrt(std::max(1, t));
        return _eta;
    }

    /// This assumes alpha = 1/2
    std::vector<double> newtons_method_weights(std::vector<double> &losses, double eta)
    {

        std::vector<double> weights;
        double x_previous = _x;
        double x_estimated = _x;
        weights.reserve(losses.size());

        for (int i = 0; i < losses.size(); i++)
            weights.push_back(0);
        do
        {
            x_previous = x_estimated;
            for (int i = 0; i < losses.size(); i++)
                weights[i] = 4 * pow((eta * (losses[i] - x_previous)), -2);
            double w_sum_powered = 0;
            double w_sum = 0;
            for (auto w : weights)
            {
                w_sum_powered += pow(w, (3 / 2));
                w_sum += w;
            }
            x_estimated = x_previous - (w_sum - 1) / (eta * w_sum_powered);
        } while (std::min(x_previous, x_estimated) / std::max(x_previous, x_estimated) >= 1.1);
        _x = x_estimated;
        return weights;
    }

public:
    explicit Tsallis_LTU(int k)
    {
        _cumulative_losses = std::vector<double>(k, 0);
        _rg = random_gen();
        _t = 0;
        _x = 1;
        _k = k;
        _eta = 1;
    }

    int choose()
    {
        if (update_triggered) {
            _weights = newtons_method_weights(_cumulative_losses, compute_eta(_t));
            _d = std::discrete_distribution(_weights.begin(), _weights.end());
        }

        update_triggered = false;
        int s = _d(_rg);

        _t += 1;
        return s;
    }

    void give_reward(size_t index, double feedback)
    {
        // We can either use IW or RV to construct the estimated reward
        // This is RW, we should also try RV, which is a reduced variance estimator
        if (1 - feedback > 0)
            update_triggered = true;

        _cumulative_losses[index] += (1 - feedback) / _weights[index];
    }
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_TSALLIS_LTU_H
