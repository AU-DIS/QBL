

#ifndef EFFICIENT_MULTI_ARMED_BANDITS_QBL_H
#define EFFICIENT_MULTI_ARMED_BANDITS_QBL_H

#include "../../utilities/updateable_priority_queue.h"
#include <math.h>
#include <algorithm>
#include <boost/random/beta_distribution.hpp>

class QBL
{
private:
    std::exponential_distribution<double> _exponential_distribution;
    std::mt19937 _gen;
    std::vector<int> _priorities;
    std::vector<double> comb_rounds_optimal;
    std::vector<int> good_rounds_in_row;
    std::discrete_distribution<int> dist;

    boost::random::beta_distribution<float> dist2;

    std::vector<double> last_term_sum_of_rewards;
    std::vector<int> last_term_sum_of_counts;
    double total_last_term_sum_of_rewards;
    int total_last_term_sum_of_counts;

    int last_choice = -1;
    std::vector<bool> was_last_choice;

    better_priority_queue::updatable_priority_queue<int, int> _q;
    long long _counter;
    int multiple = 1;
    bool even = true;
    int _logk;
    int _round;
    int rounds_leader_optimal;
    double F;

public:
    QBL() = default;
    int rand_cnt;
    int _k;
    double _eta;
    // Not used for anything just for theoretical runner generalization
    int _number_to_presample;

    std::vector<double> *get_weights()
    {
        auto ret = new std::vector<double>();
        for (auto v : _priorities)
            ret->push_back(v);
        return ret;
    }
    QBL(int m, int k, int benchmode)
    {
        rand_cnt = 0;
        _k = k;
        _eta = 0.1;
        _counter = 0;
        _round = 0;
        F = 0;
        comb_rounds_optimal = std::vector<double>(_k, 0);
        good_rounds_in_row = std::vector<int>(_k, 0);

        //Init weightings for isRewarding calsulation
        last_term_sum_of_rewards = std::vector<double>(_k,0);
        last_term_sum_of_counts = std::vector<int>(_k,1);
        total_last_term_sum_of_rewards = 0;_k;
        total_last_term_sum_of_counts = _k;

        //Init last choice for m version
        was_last_choice = std::vector<bool>(_k, false);

        dist = std::discrete_distribution<int>({0.001, 0.999});
        dist2 = boost::random::beta_distribution<float>(1.0,1.0);



        _logk = (int)log2(_k);
        //_logk = k;
        //_logk = 4;
        //_logk = 0;

        _exponential_distribution = std::exponential_distribution<double>(_eta);
        _gen = random_gen();

        _priorities = std::vector<int>();

        _q = better_priority_queue::updatable_priority_queue<int, int>();
        for (int i = 0; i < _k; i++)
        {
            _priorities.push_back(i);
            //std::shuffle(_priorities.begin(),_priorities.end(),_gen);
        }
        for (int i=0; i < _k; i++) {
            _q.push(i, _priorities[i]);
        }

        rounds_leader_optimal = 0;
    }

    QBL(int k, double eta)
    {
        rand_cnt = 0;
        _k = k;
        _eta = eta;
        _counter = 0;
        _round = 0;
        F = 0;
        comb_rounds_optimal = std::vector<double>(_k, 0);
        good_rounds_in_row = std::vector<int>(_k, 0);

        //Init weightings for isRewarding calculation
        last_term_sum_of_rewards = std::vector<double>(_k,1);
        last_term_sum_of_counts = std::vector<int>(_k,1);
        total_last_term_sum_of_rewards = _k;
        total_last_term_sum_of_counts = _k;

        //Init last choice for m version
        was_last_choice = std::vector<bool>(_k, false);

        dist = std::discrete_distribution<int>({0.001, 0.999});
        dist2 = boost::random::beta_distribution<float>(1.0,1.0);



        _logk = (int)log2(_k);
        //_logk = k;
        //_logk = 4;
        //_logk = 0;

        _exponential_distribution = std::exponential_distribution<double>(_eta);
        _gen = random_gen();

        _priorities = std::vector<int>();

        _q = better_priority_queue::updatable_priority_queue<int, int>();
        for (int i = 0; i < _k; i++)
        {
            _priorities.push_back(i);
            //std::shuffle(_priorities.begin(),_priorities.end(),_gen);
        }
        for (int i=0; i < _k; i++) {
            _q.push(i, _priorities[i]);
        }

        rounds_leader_optimal = 0;
    }

    /*QBL(const QBL &prototype)
    {
        _k = prototype._k;
        _eta = prototype._eta;
        _counter = 0;
        _round = 0;

        //_logk = (int) log2(k);
        //_logk = prototype._k;
        //_logk = 2;

        _exponential_distribution = std::exponential_distribution<double>(_eta);
        _gen = random_gen();
        _priorities = std::vector<int>();

        _q = better_priority_queue::updatable_priority_queue<int, int>();
        for (int i = 0; i < _k; i++)
        {
            _priorities.push_back(i);
            _q.push(i, i);
        }
        rounds_leader_optimal = 0;
    }*/


    void enforce_unique_priority(int updates) {
        std::vector<int> q_order(updates, 0);
        for (int i = 0; i < updates; i++) {
            q_order[i] = _q.top().key;
            _priorities[q_order[i]] = _k-i;
            _q.pop();
        }
        for (int i = 0; i < updates; i++) {
            _q.push(q_order[i], _priorities[q_order[i]]);
        }
    }

    std::vector<int> choose(int m)
    {

        /*for (int i = 0; i < _logk; i++)
        {
            int j = (_counter+i)%_k;
            int priority_delta = (int)_exponential_distribution(_gen);
            if (priority_delta != 0) {
                rand_cnt++;
                std::cout << rand_cnt++<< std::endl;
                _priorities[j] = min(_k, _priorities[j] + priority_delta);
                _q.update(j, _priorities[j]);
            }
            _counter++;
        }*/
        //enforce_unique_priority(_k);
        std::vector<int> choices(m, 0);
        for (int i = 0; i < m; i++)
        {
            choices[i] = _q.top().key;
            _q.pop();
        }
        for (int i = 0; i < m; i++)
        {
            _q.push(choices[i], _priorities[choices[i]]);
        }
        /*
        for (int i = 0; i < _logk; i++)
        {
            int priority_delta = (int)_exponential_distribution(_gen);
            _priorities[i] = min(_k, _priorities[i] + priority_delta);
            _q.update(i, _priorities[i]);
            _counter++;
        }
        std::vector<int> choices(m, 0);
        for (int i = 0; i < m; i++)
        {
            choices[i] = _q.top().key;
            _q.pop();
        }
        for (int i = 0; i < m; i++)
        {
            _q.push(choices[i], _priorities[choices[i]]);
        }
*/
        return choices;
    }
    void give_reward(std::vector<int> &indices, std::vector<double> &rewards)
    {
        _counter++;
        //if (_counter%1000 == 0) enforce_unique_priority(_k);
        /*if (_counter%30000 == 0) {
           for (int i = 0; i < _k; i++) {
               std::cout << _priorities[i] << " ";
           }
           std::cout << std::endl;
            //enforce_unique_priority(_k);
        }*/
        for (int i = 0; i < indices.size(); i++) {
            int choice = indices[i];
            double reward = rewards[i];

            if (!was_last_choice[choice]) {
                total_last_term_sum_of_rewards -= last_term_sum_of_rewards[choice]-last_term_sum_of_rewards[choice]/last_term_sum_of_counts[choice];
                last_term_sum_of_rewards[choice] /= last_term_sum_of_counts[choice];
                total_last_term_sum_of_counts -= last_term_sum_of_counts[choice]-1;
                last_term_sum_of_counts[choice] = 1;
                was_last_choice[choice] = true;
            }
            //Add to reward and count
            total_last_term_sum_of_rewards += reward;
            last_term_sum_of_rewards[choice] += reward;
            total_last_term_sum_of_counts += 1;
            last_term_sum_of_counts[choice] += 1;
        //}
        //for (int i = 0; i < indices.size(); i++)
        //{
            //int choice = indices[i];
            //double reward = rewards[i];
            //Check if rewarding
            //Compare global weighted average reward to local average reward
            double global = total_last_term_sum_of_rewards/total_last_term_sum_of_counts;
            double local = last_term_sum_of_rewards[choice]/last_term_sum_of_counts[choice];

            //dist.param({global*0.001, local});
            bool is_rewarding = global < local*(dist2(_gen)*(1.1-0.9)+0.9);//dist(_gen);//global < local*0.85*dist(_gen);


            if (!is_rewarding) {
                int new_position = last_term_sum_of_counts[choice]-1;
                new_position = std::min(new_position, _k-1-(_q.top().priority-_priorities[choice]));
                _priorities[choice] = _q.top().priority+(new_position-_k);
                _q.update(choice, _priorities[choice]);
                was_last_choice[choice] = false;
            }
        }
    }

    int choose()
    {
        /*for (int i = 0; i < _logk; i++)
        {
            int priority_delta = (int)_exponential_distribution(_gen);
            _priorities[i] = min(_k, _priorities[i] + priority_delta);
            _q.update(i, _priorities[i]);
            _counter++;
        }*/
        return _q.top().key;
    }
    void give_reward(int choice, double feedback)
    {
        _counter++;
        /*double threshold = pow(3.2, multiple);
        if (_counter > threshold)
        {
            even = !even;
            multiple++;
        }*/
        //if (_counter%1000 == 0) enforce_unique_priority(_k);
        /*if (_counter%100 == 0) {

           //for (int i = 0; i < _k; i++) {
           int min_ = *std::min_element(_priorities.begin(), _priorities.end());
           int max_ = *std::max_element(_priorities.begin(), _priorities.end());
           std::cout << min_ << " " << max_ << " " << max_ - min_ << std::endl;
           //}
           //std::cout << std::endl;
        }*/

        //Update counters if leader has changed
        if (choice != last_choice) {
            total_last_term_sum_of_rewards -= last_term_sum_of_rewards[choice]-last_term_sum_of_rewards[choice]/last_term_sum_of_counts[choice]; //-last_term_sum_of_rewards[choice]/last_term_sum_of_counts[choice];
            last_term_sum_of_rewards[choice] /= last_term_sum_of_counts[choice];// last_term_sum_of_counts[choice];
            total_last_term_sum_of_counts -= last_term_sum_of_counts[choice]-1;//-last_term_sum_of_counts[choice]/2;
            last_term_sum_of_counts[choice] = 1;
            last_choice = choice;
        }
        //Add to reward and count
        double global1 = total_last_term_sum_of_rewards/total_last_term_sum_of_counts;
        double local1 = last_term_sum_of_rewards[choice]/last_term_sum_of_counts[choice];
        total_last_term_sum_of_rewards += feedback;
        last_term_sum_of_rewards[choice] += feedback;
        total_last_term_sum_of_counts += 1;
        last_term_sum_of_counts[choice] += 1;

        //Check if rewarding
        //Compare global weighted average reward to local average reward
        double global = total_last_term_sum_of_rewards/total_last_term_sum_of_counts;
        double local = last_term_sum_of_rewards[choice]/last_term_sum_of_counts[choice];


        bool is_rewarding = global < local*(dist2(_gen)*(1.1-0.9)+0.9);

        //std::cout << is_rewarding << "  " << global << " " << local << "\n";

        if (!is_rewarding) {
            int new_position = last_term_sum_of_counts[choice]-1;//round(_k*log(last_term_sum_of_counts[choice])/log(_k));
            new_position = std::min(new_position, _k-1-(_q.top().priority-_priorities[choice]));
            _priorities[choice] = new_position-_k-1 == 0 ? _q.top().priority-1 : _q.top().priority+(new_position-_k-1);
            _q.pop();
            _q.push(choice, _priorities[choice]);
            last_choice = -1;
            /*if (choice == 0 ) {
                std::cout << even << "   " << _counter<< "  before " << comb_rounds_optimal[0];
                std::cout << "    " << 1-(1/(double)comb_rounds_optimal[0]);
            }*/

            //comb_rounds_optimal[choice] = good_rounds_in_row[choice]/comb_rounds_optimal[choice]*_k;//good_rounds_in_row[choice] <= 2 ? 0 : floor(comb_rounds_optimal[choice]*0.5);//comb_rounds_optimal[choice]*(1-(1/(double) good_rounds_in_row[choice]));//(int) (comb_rounds_optimal[choice] * 0.9);
            //good_rounds_in_row[choice] = 0;
            /*if (choice == 0 ) {
                std::cout << "  after " << comb_rounds_optimal[0] << std::endl;
            }*/

            //rounds_leader_optimal = 0;
        } //else {
            //comb_rounds_optimal[choice] += 1;
            //good_rounds_in_row[choice] += 1;
            /*if (choice == 0) {
                std::cout << even << "   " << _counter<< "  good " << comb_rounds_optimal[0] << std::endl;
            }*/

            //rounds_leader_optimal += 1;// min(rounds_leader_optimal + 1, _k - 2);
            // std::cout << std::to_string(rounds_leader_optimal) + "\n";
        //}
    }

};

#endif // EFFICIENT_MULTI_ARMED_BANDITS_QBL_H
