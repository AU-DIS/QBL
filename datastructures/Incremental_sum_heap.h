

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_SUM_HEAP_H
#define EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_SUM_HEAP_H
#include "cmath"
#include "random"
#include <stdio.h>
#include <set>
#include "../utilities/random_gen.h"

class Incremental_sum_heap
{

public:
    explicit Incremental_sum_heap(std::vector<double> &input_list)
    {
        _n = input_list.size();
        _d =  _n;//(int) pow(2, ceil(log2(_n)));
        //std::cout << "d: " << _d << std::endl;
        _heap = std::vector<double>(2 * _d, 0);
        _leafs = std::vector<double>(2 * _d, 0);
        _min = std::vector<double>(2 * _d, 0);
        _max = std::vector<double>(2 * _d, 0);
        int j = 0;
        for (int i = _d; i < _d + _n; i++)
        {
            _heap[i] = input_list[j];
            _leafs[i] = 1;
            _min[i] = input_list[j];
            _max[i] = input_list[j];
            j++;
        }
        //for (int i = _d + _n; i < 2*_d; i++) {
        //    _heap[i] = std::numeric_limits<double>::infinity();
        //}
        for (int i = _d-1; i > 0; i--)
        {
            if (std::isinf(_heap[2*i+1])) {
                _heap[i] = _heap[2*i];
                _leafs[i] = _leafs[2*i];
                _min[i] = _min[2*i];
                _max[i] = _max[2*i];
            } else if (_heap[2 * i] > _heap[2 * i + 1]) {
                _heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            } else {
                _heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            }
            //if (fabs(_heap[i]) < 1e-15) {
            //    _heap[i] = 0;
            //}
        }
        //std::cout <<"leafcount: " << _leafs[1] << std::endl;
        //std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4] << " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
    }
    explicit Incremental_sum_heap(std::vector<double> &input_list, double eta)
    {
        _n = input_list.size();
        _d =  _n;//(int) pow(2, ceil(log2(_n)));
        //std::cout << "d: " << _d << std::endl;
        _heap = std::vector<double>(2 * _d, 0);
        _leafs = std::vector<double>(2 * _d, 0);
        _min = std::vector<double>(2 * _d, 0);
        _max = std::vector<double>(2 * _d, 0);
        _eta = eta;
        int j = 0;
        for (int i = _d; i < _d + _n; i++)
        {
            _heap[i] = -_eta*input_list[j];
            _leafs[i] = 1;
            _min[i] = -_eta*input_list[j];
            _max[i] = -_eta*input_list[j];
            j++;
        }
        //for (int i = _d + _n; i < 2*_d; i++) {
        //    _heap[i] = std::numeric_limits<double>::infinity();
        //}
        for (int i = _d-1; i > 0; i--)
        {
            if (std::isinf(_heap[2*i+1])) {
                _heap[i] = _heap[2*i];
                _leafs[i] = _leafs[2*i];
                _min[i] = _min[2*i];
                _max[i] = _max[2*i];
            } else if (_heap[2 * i] > _heap[2 * i + 1]) {
                _heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            } else {
                _heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            }
            //if (fabs(_heap[i]) < 1e-15) {
            //    _heap[i] = 0;
            //}
        }
        //std::cout <<"leafcount: " << _leafs[1] << std::endl;
        //std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4] << " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
    }

    Incremental_sum_heap() = default;
    ;
    void update_loss(int k, double v) {
        //int d = _heap.size();
        int i = _d + k;
        _heap[i] = (std::isinf(v)) ? std::numeric_limits<double>::infinity() : -_eta*v;
        _leafs[i] = (std::isinf(v)) ? 0 : 1;
        _max[i] = v;
        _min[i] = v;
        while (i > 0)
        {
            i /= 2;
            if (std::isinf(_heap[2*i+1])) {
                _heap[i] = _heap[2*i];
                _leafs[i] = _leafs[2*i];
                _max[i] = _max[2*i];
                _min[i] = _min[2*i];
            } else if (std::isinf(_heap[2*i])) {
                _heap[i] = _heap[2*i+1];
                _leafs[i] = _leafs[2*i+1];
                _max[i] = _max[2*i+1];
                _min[i] = _min[2*i+1];
            } else {
                _heap[i] = _heap[2 * i +1] + _heap[2*i];
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            }
        }

    }


    void update(int k, double v)
    {
        //int d = _heap.size();
        int i = _d + k;
        _heap[i] = v;
        _leafs[i] = (std::isinf(v)) ? 0 : 1;
        while (i > 0)
        {
            i /= 2;
            if (std::isinf(_heap[2*i+1])) {
                _heap[i] = _heap[2*i];
                _leafs[i] = _leafs[2*i];
                _max[i] = _max[2*i];
                _min[i] = _min[2*i];
            } else if (std::isinf(_heap[2*i])) {
                _heap[i] = _heap[2*i+1];
                _leafs[i] = _leafs[2*i+1];
                _max[i] = _max[2*i+1];
                _min[i] = _min[2*i+1];
            } else if (_heap[2 * i] > _heap[2 * i + 1]) {
                _heap[i] = _heap[2 * i] + log(exp(_heap[2 * i + 1]-_heap[2 * i])+1);
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            } else {
                _heap[i] = _heap[2 * i + 1] + log(exp(_heap[2 * i]-_heap[2 * i + 1])+1);
                _leafs[i] = _leafs[2*i]+_leafs[2*i+1];
                _min[i] = (_min[2*i] < _min[2*i+1]) ? _min[2*i] : _min[2*i+1];
                _max[i] = (_max[2*i] > _max[2*i+1]) ? _max[2*i] : _min[2*i+1];
            }
            //if (fabs(_heap[i]) < 1e-15) {
            //    _heap[i] = 0;
            //}
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
        double p = _uni(_random_gen);//log(_uni(_random_gen)) + _heap[1];
        int i = 1;
        while (i < _d)
        {
            i *= 2;
            if (std::isinf(_heap[i])) {
                i += 1;
                continue;
            }
            if (std::isinf(_heap[i+1])) {
                continue;
            }

            double left = exp(_heap[i]-_heap[1]);
            double right = _heap[i+1];

            if (p > left)
            {
                //double exp_val = exp(left-p);
                //if (exp_val >= 1-1e-15) {
                //    p = p+log(1e-15);
                //} else {
                //    p = p+log(1-exp_val);
                //}
                //-= left;   log(a)+log(1-exp(log(b)-log(a)))
                p -= left;
                i += 1;
                //std::cout << "go right" << std::endl;
            } else {
                //std::cout << "go left" << std::endl;
            }
        }
        //std::cout << i << " " << i-_d << std::endl;

        return i - _d;
    }

    double get_heap_min() {
        return _min[1];
    }

    double get_heap_max() {
        return _max[1];
    }

    int heap_loss_sample()
    {
        double p = _uni(_random_gen);//log(_uni(_random_gen))+_heap[1];//1-exp((log(_uni(_random_gen))+_heap[1])-_heap[1]);//_heap[1]+ log(1-exp((log(_uni(_random_gen)) + _heap[1])-_heap[1]));
        std::cout << "Start p: " << p << std::endl;
        int i = 1;
        while (i < _d)
        {
            i *= 2;
            if (std::isinf(_heap[i])) {
                i += 1;
                continue;
            }
            if (std::isinf(_heap[i+1])) {
                continue;
            }
            double left = exp(_heap[i])/exp(_heap[1]);

            //double top_left = (_leafs[i] == 1) ? exp(-0.1*_heap[i]): exp(log(_leafs[i])+(-0.1*_min[i]));
            //double top_right = (_leafs[i+1] == 1) ? exp(-0.1*_heap[i+1]): exp(log(_leafs[i+1])+(-0.1*_min[i+1]));
            //double left =  top_left/SE; //_heap[1] + log(1-exp(_heap[i]-_heap[1]));
            double right = exp(_heap[i+1]-_heap[1]); //log(_leafs[i+1])+(-0.1*_min[i+1]);
            std::cout << p << " " << left << " " << right << " SUM: " << left+right<< std::endl;

            if (p > left)
            {
                p -= left; // _heap[i]+log(1-exp(p-_heap[i]));
                //double exp_val = exp(left-p);
                //if (exp_val >= 1-1e-15) {
                //    p = p+log(1e-15);
                //} else {
                //p = p+log(1-exp_val);
                //}
                //-= left;   log(a)+log(1-exp(log(b)-log(a)))

                i += 1;
                //std::cout << "go right" << std::endl;
            } else {
                //std::cout << "go left" << std::endl;
            }
        }
        //std::cout << i << " " << i-_d << std::endl;
        print_internals();
        print_weights();
        std::cout << std::endl;

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

    int heap_random_sample() {
        double p = _uni(_random_gen)*_leafs[1];
        //std::cout << _heap[1] << " " << _heap[2] << " " << _heap[3] << " " << _heap[4]<< " " << _heap[5] << " " << _heap[6] << " " << _heap[7] << std::endl;
        int i = 1;
        while (i < _d)
        {
            i *= 2;
            if (std::isinf(_leafs[i])) {
                i += 1;
                continue;
            }
            if (std::isinf(_leafs[i+1])) {
                continue;
            }

            double left = _leafs[i];

            if (p > left)
            {
                p -= left;

                i += 1;
            }
        }

        return i - _d;
    };

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
            if (_uni(_random_gen)<0.1) {
                v = rand()%arm_set.size();
                auto iter = std::begin(arm_set);
                std::advance(iter,v);
                int choice = *iter;
                arm_set.erase(choice);
            } else {
                v = heap_sample();
            }
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
    void print_internals() {
        for (int i = 1; i < _d; i++)
        {
            std::cout << _heap[i] << ", ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<double> _heap;
    int _n;
    int _d;
    double _eta;
    std::vector<double> _min;
    std::vector<double> _max;
    std::vector<double> _leafs;
    std::uniform_real_distribution<double> _uni;
    std::mt19937 _random_gen = random_gen();
};

#endif //EFFICIENT_MULTI_ARMED_BANDITS_INCREMENTAL_SUM_HEAP_H
