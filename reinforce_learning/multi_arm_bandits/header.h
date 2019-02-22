#ifndef HEADER_H
#define HEADER_H

#include <cstddef>        //size_t
#include <cstdint>        // intxx_t, uintxx_t
#include <linux/types.h>  // ssize_t

namespace rl {
    typedef ssize_t    size_type;
    typedef double     reward_type;
}

#endif
