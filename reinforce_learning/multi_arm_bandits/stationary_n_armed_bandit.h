#ifndef STATIONARY_N_ARMED_BANDIT_H
#define STATIONARY_N_ARMED_BANDIT_H

#include <vector>
#include "header.h"
#include "action.h"

namespace rl {
    class StationaryNArmedBandit {
    public:
        StationaryNArmedBandit(size_type n = 10);
        reward_type selection(const size_type&); 
    private:
        std::default_random_engine  _e;        // random engine used throughout program.
        size_type                   _n;        // the number of actions.
        std::vector<Action>         _actions;
    };

    StationaryNArmedBandit::StationaryNArmedBandit(size_type n)
    : _e(std::default_random_engine(std::time(0))), 
      _n(n), _actions(std::vector<Action>()) {
        for (int i = 0; i < n; ++i)
            _actions.push_back(Action(_e));
    }

    reward_type
    StationaryNArmedBandit::selection(const size_type& n) {
        if (n < 0 || n >= _n)
            return -1.0;
        return _actions[n].reward(_e);
    }
}

#endif
