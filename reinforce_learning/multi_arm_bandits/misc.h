#ifndef MISC_H
#define MISC_H

#include <utility>       // move()
#include <functional>    // less(), less_equal(), ...
#include "header.h"

namespace rl {
    /* @fn index_min_max()
     * Find the index of a maximal or minimal value in a sequence.
     */
    template <class Iter, typename Cmp>
    size_type
    index_min_max(const Iter& b, const Iter& e, Cmp cmp) {
        if (b == e)
            return -1;
        auto temp = *b;
        size_type m = 0, cnt = 1;
        for (Iter iter = b + 1; iter != e; ++iter, ++cnt) {
            if (cmp(*iter, temp)) {
                temp = *iter; m = cnt;
            }
        }
        return m;
    }

    template <class Iter>
    size_type
    index_of_min(const Iter& b, const Iter& e) {
        return index_min_max(b, e, std::less<decltype(*b)>());
    }

    template <class Iter>
    size_type
    index_of_max(const Iter& b, const Iter& e) {
        return index_min_max(b, e, std::greater<decltype(*b)>());
    }
}

#endif
