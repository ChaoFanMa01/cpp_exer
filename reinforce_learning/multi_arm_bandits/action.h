#ifndef ACTION_H
#define ACTION_H

#include <random>
#include <ctime>
#include "header.h"

namespace rl {
    /* @class Action
     * Represent an action in a n-armed bandit.
     */
    class Action {
    public:
        typedef double    probability_type;

        Action(std::default_random_engine&);
        ~Action() = default;

        reward_type reward(std::default_random_engine&) const;
    private:
        static const reward_type    max;
        reward_type                      value;  // the value of this action.
    };

    const reward_type Action::max = 10.0;

    Action::Action(std::default_random_engine& e) {
        // standard normal distribution with mean = 5.0, standard deviation = 2.0.
        std::normal_distribution<double> dis(5.0, 2.0);
        while ((value = dis(e)) < 0.0 || value > max) ;
    }

    reward_type
    Action::reward(std::default_random_engine& e) const {
        std::normal_distribution<double> dis(5.0, 2.0);
        double   r;
        do {
            r = value + dis(e);
        } while (r < 0.0 || r > max);
        return r;
    }
}

#endif
