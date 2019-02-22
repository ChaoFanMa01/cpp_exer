#ifndef GREEDY_H
#define GREEDY_H

#include <iostream>
#include <vector>
#include "header.h"
#include "stationary_n_armed_bandit.h"
#include "misc.h"

namespace rl {
    /* @class Greedy 
     * Implement the greedy method for n-armed bandit problem.
     */
    class Greedy {
    public:
        Greedy(size_type, size_type);
        void run(StationaryNArmedBandit&);
        void print_result();
    private:
        void update_estimates();
    private:
        /* the historical rewards. */
        std::vector<reward_type>    _values;
        /* the estimated values for current step. */
        std::vector<reward_type>    _estimates;
        /* the times of an arm been selected. */
        std::vector<size_type>      _times;
        /* the record of reward returned each step. */
        std::vector<reward_type>    _rewards;
        /* the number of total iterations. */
        size_type                   _iterations;
    };

    Greedy::Greedy(size_type size, size_type iterations)
    : _values(std::vector<reward_type>()),
      _estimates(std::vector<reward_type>()),
      _times(std::vector<size_type>()),
      _rewards(std::vector<reward_type>()),
      _iterations(iterations){
        for (int i = 0; i < size; ++i) {
            _values.push_back(0.0);
            _estimates.push_back(0.0);
            _times.push_back(1.0);
        }
        for (int i = 0; i < _iterations; ++i)
            _rewards.push_back(5.0);
    }

    void
    Greedy::update_estimates() {
        /* calculate expected rewards (update _estimates) */
        for (size_type i = 0; i < _estimates.size(); ++i)
            _estimates[i] = _estimates[i] + (1.0 / _times[i]) * (_rewards[i] - _estimates[i]);
    }

    void
    Greedy::run(StationaryNArmedBandit& arms) {
        for (size_type i = 0; i < _iterations; ++i) {
            /* update expected rewards according to records. */
            update_estimates();
            /* find the one with maximal expected reward. */
            auto max = index_of_max(_estimates.begin(), _estimates.end());
            /* get the reward from the arm with maximal expected reward. */
            auto reward = arms.selection(max);
            _values.push_back(reward);
            /* update rewards. */
            _rewards[max] = reward;
            /* update selected time. */
            ++_times[max];
        }
    }

    void
    Greedy::print_result() {
        for (auto &v : _values)
            std::cout << v << " ";
        std::cout << std::endl;
    }
}

#endif
