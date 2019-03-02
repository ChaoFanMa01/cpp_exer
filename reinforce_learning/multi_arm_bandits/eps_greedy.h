#ifndef EPSI_GREEDY_H
#define EPSI_GREEDY_H

#include <random>
#include <ctime>
#include "greedy.h"
#include "roulette_wheel.h"

namespace rl {
    class EpsiGreedy: public Greedy {
    public:
        EpsiGreedy(size_type, size_type, double);
        ~EpsiGreedy() = default;

        void run(StationaryNArmedBandit&);
    private:
        double _threshold;
    };

    EpsiGreedy::EpsiGreedy(size_type size, size_type iterations, 
               double thre)
    : Greedy(size, iterations), _threshold(thre) {}

    void 
    EpsiGreedy::run(StationaryNArmedBandit& arms) {
        std::uniform_int_distribution<unsigned> u(0, _times.size() - 1);
        std::default_random_engine e(std::time(0));
        RouletteWheel wheel(_threshold);

        for (size_type i = 0; i < _iterations; ++i) {
            /* update expected rewards according to records. */
            update_estimates();
            /* find the one with maximal expected reward. */
            size_type max;
            if (!wheel.run()) {
                max = index_of_max(_estimates.begin(), _estimates.end());
            } else {
                max = u(e);
            }
            /* get the reward from the arm with maximal expected reward. */
            auto reward = arms.selection(max);
            _values.push_back(reward);
            /* update rewards. */
            _rewards[max] = reward;
            /* update selected time. */
            ++_times[max];
        }
    }
}

#endif
