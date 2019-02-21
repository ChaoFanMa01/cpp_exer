#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include "header.h"
#include "stationary_n_armed_bandit.h"

namespace rl {
    /* @class Greedy 
     * Implement the greedy method for n-armed bandit problem.
     */
    class Greedy {
    public:
        Greedy(StationaryNArmedBandit&, size_type&, size_type&);
        void run();
    private:
        /* record the average values of n arms. */
        std::vector<reward_type>    _values;
        /* the estimated values for current step. */
        std::vector<reward_type>    _estimates;
        /* the times of an arm been selected. */
        std::vector<size_type>      _times;
        /* the record of reward returned each step. */
        std::vector<reward_type>    _rewards;
    };

    Greedy::Greedy(StationaryNArmedBandit& arms, size_type& size, size_type& iteration)
    : _values(std::vector<reward_type>()),
      _estimates(std::vector<reward_type>()),
      _times(std::vector<size_type>()),
      _rewards(std::vector<reward_type>()){
        for (int i = 0; i < size; ++i) {
            _values.push_back(0);
            _estimates.push_back(0);
            _times.push_back(0);
        }
    }

    void
    run() {
        
    }
}

#endif
