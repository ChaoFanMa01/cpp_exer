#include <iostream>
#include <random>
#include <ctime>

#include "header.hpp"

void encode_test(void) {
    std::uniform_int_distribution<int> ui(-100, 100);
    std::default_random_engine e(std::time(0));
    std::vector<bool>    str;
    int                  tmp;

    for (int i = 0; i < 10; ++i) {
        tmp = ui(e);
        str = genetic::encode(tmp, 20);
        if (!str.empty()) {
            std::cout << tmp << ": ";
            if (str[0])
                std::cout << "-";
            else 
                std::cout << "+";
            for (int i = 1; i < str.size(); ++i)
                std::cout << str[i];
            std::cout << std::endl;
        }
        std::cout << "decode: " << genetic::decode(str) << std::endl;
    }
}

genetic::fitness_type triple(genetic::value_type val) {
    return std::pow(val, 3) - 60 * std::pow(val, 2) + 900 * val + 100;
}

void GA_test(void) {
    genetic::GA     gal(10, 0, 30, 40, triple);

    gal.prepare();
    gal.run();
    std::cout << "optimal: " << gal.get_max() << std::endl;
}

int
main(void) {
    GA_test();
    return 0;
}
