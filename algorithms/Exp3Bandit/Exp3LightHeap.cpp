
#include <vector>
#include "cmath"
#include "algorithm"
#include "iostream"
#include "Exp3LightHeap.h"
#include "../../utilities/argsort.h"
#include "../../datastructures/Incremental_LSE_sum_heap.h"
#include "../../datastructures/Incremental_sum_heap.h"
#include "../../datastructures/Incremental_product_heap.h"
//#include "../utilities/random_gen.h"

#include "cmath"




Exp3LightHeap::Exp3LightHeap(int k, double eta) : _k(k), _eta(eta)
{
    _weights = std::vector<double>(k, 1.0);
    _distribution = Incremental_sum_heap(_weights);
    _last_drawn_weight=0;
    _last_drawn_probability=1;
};
int Exp3LightHeap::choose()
{
    //if (_uni(_random_gen)<0.1) {
    //    a = _distribution.heap_random_sample();
    //} else {
    int a = _distribution.heap_sample();
    //}
    _last_drawn_weight = a;
        //std::cout << i << " ";
        _last_drawn_probability = exp(_weights[a] - _distribution.max_element());

    //std::cout << std::endl;
    _distribution.update(a, std::numeric_limits<double>::infinity());
    return a;
}

void Exp3LightHeap::give_reward(int index, double reward)
{
    _weights[index] += -_eta*((1-reward) /_last_drawn_probability); //implicit log

    _distribution.update(index, _weights[index]);
}

std::vector<double> *Exp3LightHeap::get_weights()
{
    return &_weights;
}

