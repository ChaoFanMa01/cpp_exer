#include <random>
#include <ctime>
#include "binary_search_tree.hpp"

int
main(void) {
    std::default_random_engine e(std::time(0));
    std::uniform_int_distribution<int> u(0, 100);
    my::binary_search_tree<int> bst;

    for (int i = 0; i < 100; ++i) {
        bst.insert(u(e));
    }

    bst.print();

    return 0;
}
