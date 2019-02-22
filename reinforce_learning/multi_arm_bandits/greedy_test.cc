#include <iostream>

#include "header.h"
#include "stationary_n_armed_bandit.h"
#include "greedy.h"

int
main() {
    rl::StationaryNArmedBandit arms(10);
    rl::Greedy greedy(10, 100);

    greedy.run(arms);

    greedy.print_result();

    return 0;
}
