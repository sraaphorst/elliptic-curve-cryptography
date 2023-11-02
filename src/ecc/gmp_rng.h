/**
 * rand_gmp.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <chrono>
#include <gmp.h>

#include "big_int.h"

namespace ecc::gmp {
    // A random number generator.
    class gmp_rng {
    private:
        gmp_randstate_t state;

    public:
        gmp_rng() {
            const auto time = std::chrono::high_resolution_clock::now();
            const auto duration = time.time_since_epoch();
            const auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

            gmp_randinit_mt(state);
            gmp_randseed_ui(state, seed);
        }

        BigInt random_mod(const BigInt&);

        ~gmp_rng() {
            gmp_randclear(state);
        }
    };
}
