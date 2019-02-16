#include <iostream>
#include "header.h"
#include "stationary_n_armed_bandit.h"

int main() {
    rl::StationaryNArmedBandit bandit(10);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j)
            std::cout << bandit.selection(i) << " ";
        std::cout << std::endl;
    }

    return 0;
}
