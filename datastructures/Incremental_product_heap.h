

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_PRODUCT_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_PRODUCT_HEAP_H
#include "cmath"
#include "random"
#include <stdio.h>
#include "../utilities/random_gen.h"

class Incremental_product_heap
{

public:
    explicit Incremental_product_heap(std::vector<double> &input_list)
    {
        _n = input_list.size();
        _d = round(pow(2, log2(_n)));
        _heap = std::vector<double>(2 * _d, 1);
        int j = 0;
        for (int i = _d; i < _d + _n; i++)
        {
            _heap[i] = input_list[j];
            j++;
        }
        for (int i = _d - 1; i >= 1; i--)
        {
            _heap[i] = _heap[2 * i] * _heap[2 * i + 1];
        }
    }
    Incremental_product_heap() = default;
    ;

    void update(int k, double v)
    {
        int d = _heap.size();
        int i = d / 2 + k;
        _heap[i] = v;
        while (i > 0)
        {
            i /= 2;
            _heap[i] = _heap[2 * i] * _heap[2 * i + 1];
        }
    }

    double probability_of_choice(int i)
    {
        return _heap[_heap.size() / 2 + i];
    }
    double max_element()
    {
        return _heap[1];
    }

    int heap_sample()
    {
        double p = _uni(_random_gen) * _heap[1];

        int i = 1;
        while (i < _d)
        {
            i *= 2;
            double left = _heap[i];
            if (p > left)
            {
                p -= left;
                i += 1;
            }
        }
        return i - _d;
    }

    std::tuple<int, bool> heap_sample_observe(double _gamma)
    {
        double p = _uni(_random_gen) * _heap[1];

        int i = 1;
        while (i < _d)
        {
            i *= 2;
            double left = _heap[i];
            if (p > left)
            {
                p -= left;
                i += 1;
            }
        }

        bool gamma_obs = (_heap[i] < _heap[1]*_gamma);
        //std::cout << _heap[i] << "        " << _heap[1]/(float)_n << std::endl;
        return {i - _d, gamma_obs};
    }

    std::vector<int> heap_sample(int m)
    {
        auto choices = std::vector<int>(m, 0);
        auto priors = std::vector<double>(m, 1.);
        auto z = std::vector<int>(m, 0);
        for (int i = 0; i < m; i++)
        {
            int v = heap_sample();
            choices[i] = v;
            priors[i] = probability_of_choice(v);
            z[i] = v;
            update(v, 1);
        }
        for (int i = 0; i < m; i++)
        {
            update(choices[i], priors[i]);
        }
        return z;
    }

    void print_weights()
    {
        for (int i = _d; i < _d + _n; i++)
        {
            std::cout << _heap[i] << ", ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<double> _heap;
    int _n;
    int _d;
    std::uniform_real_distribution<double> _uni;
    std::mt19937 _random_gen = random_gen();
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_PRODUCT_HEAP_H
