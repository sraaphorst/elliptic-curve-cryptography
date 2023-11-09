/**
 * rand_gmp.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <gmp.h>

#include "big_int.h"

namespace ecc::gmp {
    // A random number generator.
    class gmp_rng {
    private:
        gmp_randstate_t state;

    public:
        gmp_rng() noexcept;
        ~gmp_rng() noexcept;

        // Generate a BigInt in [0,mod), where mod is the parameter.
        BigInt random_mod(const BigInt&) noexcept;
    };
}
