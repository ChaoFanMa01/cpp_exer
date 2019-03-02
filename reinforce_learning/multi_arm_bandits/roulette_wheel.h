#ifndef ROULETTE_WHEEL_H
#define ROULETTE_WHEEL_H

#include <random>
#include <ctime>

namespace rl {
    class RouletteWheel {
    public:
        RouletteWheel(double);
        bool run();
    private:
        /* the victory threshold of this roulette wheel */
        double                                  _threshold;
        std::uniform_real_distribution<double>  _rand;
        std::default_random_engine              _engine;
    };

    RouletteWheel::RouletteWheel(double thr)
    : _threshold(thr), 
      _rand(std::uniform_real_distribution<double>(0.0,1.0)),
      _engine(std::default_random_engine(time(0))){
    }

    bool 
    RouletteWheel::run() {
        double rand = _rand(_engine);
        if (rand <= _threshold)
            return true;
        return false;
    }
}

#endif
