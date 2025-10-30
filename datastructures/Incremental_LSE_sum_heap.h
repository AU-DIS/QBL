
#ifndef EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_LSE_SUM_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_LSE_SUM_HEAP_H
#include "cmath"
#include "random"
#include <stdio.h>
#include <set>
#include "../utilities/random_gen.h"

class Incremental_LSE_sum_heap
{

public:
    explicit Incremental_LSE_sum_heap(std::vector<double> &input_list)
    {
        _n = input_list.size();
        _d = _n;  //(int) pow(2, ceil(log2(_n)));

        //std::cout << "d: " << _d << std::endl;
        _max = std::vector<double>(_d*2,0);
        _heap = std::vector<double>(_d*2, 0);
        int j = 0;
        for (int i = _d; i < _d + _n; i++)
        {
            _heap[i] = 1; //input_list[j];
            _max[i] = input_list[j];
            j++;
        }
        //for (int i = _d + _n; i < _d*2; i++) {
        //    _heap[i] = std::numeric_limits<double>::infinity();
        //}
        for (int i = _d-1; i > 0; i--)
        {
            if (std::isinf(_max[2*i+1])) {
                _heap[i] = _heap[2*i];
                _max[i] = _max[2*i];
            } else if (_max[2 * i] > _max[2 * i + 1]) {
                //_heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
                _max[i] = _max[2 * i];
                double adjusted = _heap[2 * i + 1]*exp(_max[2 * i + 1] - _max[2 * i]);
                //if (adjusted < 1e-3) {
                //    adjusted = 1e-3;
                //}
                _heap[i] = adjusted + _heap[2 * i];
            } else {
                _max[i] = _max[2*i+1];
                double adjusted = _heap[2 * i]*exp(_max[2 * i] - _max[2 * i + 1]);
                //if (adjusted < 1e-3) {
                //    adjusted = 1e-3;
                //}
                _heap[i] = adjusted + _heap[2 * i + 1];

                //_heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
            }
            /*if (fabs(_heap[i]) < 1e-15) {
                _heap[i] = 0;
            }*/
        }
        std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4] << " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
        std::cout << _max[1] << " " << _max[2] << " " << _max[3] << " " << _max[4] << " " << _max[5] << " " << _max[6] << " " << _max[7] << std::endl;


    }
    Incremental_LSE_sum_heap() = default;
    ;

    void update(int k, double v)
    {
        //int d = _heap.size();
        //std::cout << "Update val" << v << std::endl;
        int i = _d + k;
        _max[i] = v;
        while (i > 0)
        {
            i /= 2;
            if (std::isinf(_max[2*i+1])) {
                _heap[i] = _heap[2*i];
                _max[i] = _max[2*i];
            } if (std::isinf(_max[2*i])) {
                _heap[i] = _heap[2*i+1];
                _max[i] = _max[2*i+1];
            } else if (_max[2 * i] > _max[2 * i + 1]) {
                //_heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
                _max[i] = _max[2 * i];
                double adjusted = _heap[2 * i + 1]*exp(_max[2 * i + 1] - _max[2 * i]);
                //if (adjusted < 1e-3) {
                //    adjusted = 1e-3;
                //}
                _heap[i] = adjusted + _heap[2 * i];
            } else {
                _max[i] = _max[2*i+1];
                double adjusted = _heap[2 * i]*exp(_max[2 * i] - _max[2 * i + 1]);
                //if (adjusted < 1e-3) {
                //    adjusted = 1e-3;
                //}
                _heap[i] = adjusted + _heap[2 * i + 1];

                //_heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
            }
            /*if (fabs(_heap[i]) < 1e-15) {
                _heap[i] = 0;
            }*/
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
        double p = _uni(_random_gen) * _heap[1];
        //std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4]<< " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
        //std::cout << _max[1] << " " << _max[2] << " " << _max[3] << " " << _max[4] << " " << _max[5] << " " << _max[6] << " " << _max[7] << std::endl;
        //std::cout << std::endl;
        int i = 1;
        while (i < _d)
        {
            double max = _max[i];
            double left = _heap[i]-_heap[i*2+1]; //up adjusted left assuming right holds max
            i *= 2;
            if (std::isinf(_max[i])) {
                i += 1;
                continue;
            }
            if (std::isinf(_max[i+1])) {
                continue;
            }
            if (max == _max[i]) {
                left = _heap[i];
            }
            //double left = _heap[i];

            if (p > left)
            {
                p -= left;
                //p *= (1-exp(left-p));
                //Adjust probe to right max
                if (max == _max[i]) {
                    double adjusted = p*exp(max - _max[i+1]);
                    //if (adjusted < 1e-3) {
                    //    adjusted = 1e-3;
                    //}
                    p = adjusted;
                }
                //double exp_val = exp(left-p);
                /*if (exp_val >= 0.9999) {
                    p = p+log(1-0.9999);
                } else {*/
                    //p = p+log(1-exp_val);
                //}
                //-= left;   log(a)+log(1-exp(log(b)-log(a)))

                i += 1;
                //std::cout << "go right" << std::endl;
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

    std::vector<int> heap_sample(int m)
    {
        auto choices = std::vector<int>(m, 0);
        auto priors = std::vector<double>(m, 0.);
        auto z = std::vector<int>(m, 0);
        std::set<int> arm_set;
        for (int i = 0; i < _n; i++) {
            arm_set.insert(i);
        }

        for (int i = 0; i < m; i++)
        {
            int v;
            /*if (_uni(_random_gen)<0.1) {
                v = rand()%arm_set.size();
                auto iter = std::begin(arm_set);
                std::advance(iter,v);
                int choice = *iter;
                arm_set.erase(choice);
            } else {*/
                v = heap_sample();
            //}
            choices[i] = v;
            priors[i] = probability_of_choice(v);
            z[i] = v;
            update(v, std::numeric_limits<double>::infinity());
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
    std::vector<double> _max;
    std::uniform_real_distribution<double> _uni;
    std::mt19937 _random_gen = random_gen();
};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_LSE_SUM_HEAP_H
