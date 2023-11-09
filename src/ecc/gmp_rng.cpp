/**
 * rand_gmp.cpp
 * By Sebastian Raaphorst, 2023.
 */
#include <chrono>

#include <gmp.h>
#include "gmp_rng.h"
#include "big_int.h"

namespace ecc::gmp {
    gmp_rng::gmp_rng() noexcept {
        const auto time = std::chrono::high_resolution_clock::now();
        const auto duration = time.time_since_epoch();
        const auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

        gmp_randinit_mt(state);
        gmp_randseed_ui(state, seed);
    }

    gmp_rng::~gmp_rng() noexcept {
        gmp_randclear(state);
    }

    BigInt gmp_rng::random_mod(const ecc::BigInt &mod) noexcept {
        mpz_t random_num;
        mpz_init(random_num);
        const auto &e = static_cast<const mpz_t&>(mod);
        mpz_urandomm(random_num, state, e);
        BigInt result{random_num};
        mpz_clear(random_num);
        return result;
    }
}
