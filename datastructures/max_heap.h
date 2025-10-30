

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_MAX_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_MAX_HEAP_H
#include "cmath"
#include "random"
#include <stdio.h>
#include <set>
#include "../utilities/random_gen.h"

class Max_heap
{

public:
    explicit Max_heap(std::vector<double> &input_list)
    {
        _n = input_list.size();
        _d = (int) pow(2, ceil(log2(_n)));
        std::cout << "d: " << _d << std::endl;
        _heap = std::vector<double>(2 * _d, 0);
        int j = 0;
        for (int i = _d; i < _d + _n; i++)
        {
            _heap[i] = input_list[j];
            j++;
        }
        for (int i = _d + _n; i < 2*_d; i++) {
            _heap[i] = std::numeric_limits<double>::infinity();
        }
        for (int i = _d-1; i > 0; i--)
        {
            if (std::isinf(_heap[2*i+1])) {
                _heap[i] = _heap[2*i];
            } else if (_heap[2 * i] > _heap[2 * i + 1]) {
                _heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
            } else {
                _heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
            }
            if (fabs(_heap[i]) < 1e-15) {
                _heap[i] = 0;
            }
        }
        std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4] << " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
    }
    Incremental_sum_heap() = default;
    ;

    void update(int k, double v)
    {
        //int d = _heap.size();
        int i = _d + k;
        _heap[i] = v;
        while (i > 0)
        {
            i /= 2;
            if (std::isinf(_heap[2*i+1])) {
                _heap[i] = _heap[2*i];
            } else if (std::isinf(_heap[2*i])) {
                _heap[i] = _heap[2*i+1];
            } else if (_heap[2 * i] > _heap[2 * i + 1]) {
                _heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
            } else {
                _heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
            }
            if (fabs(_heap[i]) < 1e-15) {
                _heap[i] = 0;
            }
            // _heap[2 * i + 1];
        }
    }

    double probability_of_choice(int i)
    {
        return _heap[_d + i];
    }
    double max_element()
    {
        return _heap[1];
    }

    int heap_sample()
    {
        double p = log(_uni(_random_gen)) + _heap[1];
        //std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4]<< " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
        int i = 1;
        while (i < _d)
        {
            i *= 2;
            double left = _heap[i];
            double right = _heap[i+1];

            if (p > left)
            {
                double exp_val = exp(left-p);
                if (exp_val >= 0.9999) {
                    p = p+log(1-0.9999);
                } else {
                    p = p+log(1-exp_val);
                }
                //-= left;

                i += 1;
                //std::cout << "go right" << std::endl;
            } else {
                //std::cout << "go left" << std::endl;
            }
        }
        //std::cout << "end" << std::endl;

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
            } else {

            }
        }

        bool gamma_obs = (_heap[i] < _heap[1]*_gamma);
        //std::cout << _heap[i] << "        " << _heap[1]/(float)_n << std::endl;
        return {i - _d, gamma_obs};
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
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_MAX_HEAP_H
