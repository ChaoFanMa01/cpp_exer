#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include "header.h"
#include "misc.h"

int
main() {
    std::uniform_int_distribution<unsigned> dis(0, 100);
    std::default_random_engine e(std::time(0));
    std::vector<unsigned int> uints;

    for (int i = 0; i < 20; ++i)
        uints.push_back(dis(e));

    for (auto & u : uints)
        std::cout << u << " ";
    std::cout << std::endl;
    
//    auto index = rl::index_of_min(uints.begin(), uints.end());
    auto index = rl::index_of_max(uints.begin(), uints.end());

    std::cout << uints[index] << std::endl;

    return 0;
}
