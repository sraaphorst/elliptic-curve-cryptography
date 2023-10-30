/**
 * rand_gmp.cpp
 * By Sebastian Raaphorst, 2023.
 */
#include <gmp.h>
#include "gmp_rng.h"
#include "big_int.h"

namespace ecc::gmp {
    BigInt gmp_rng::random_mod(const ecc::BigInt &mod) {
        mpz_t random_num;
        mpz_init(random_num);
        const auto &e = static_cast<const mpz_t&>(mod);
        mpz_urandomm(random_num, state, e);
        BigInt result{random_num};
        mpz_clear(random_num);
        return result;
    }
}
