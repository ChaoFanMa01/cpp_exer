#include <iostream>

#include "header.h"
#include "stationary_n_armed_bandit.h"
#include "eps_greedy.h"

int
main() {
    rl::StationaryNArmedBandit arms(10);
    rl::EpsiGreedy greedy(10, 100, 0.1);

    greedy.run(arms);

    greedy.print_result();

    return 0;
}
