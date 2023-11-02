/**
 * gmp_ops.h
 * By Sebastian Raaphorst, 2023.
 *
 * There is NO guarantee that these will not break in future versions of GMP.
 * For now, they avoid allocating new data structures.
 *
 * Reference: https://gmplib.org/list-archives/gmp-discuss/2018-May/006241.html
 */

#pragma once

#include <gmp.h>

namespace ecc::gmp_ops {
    static inline void mpz_move(mpz_t x, mpz_t y) {
        *x = *y;
    }

    static inline void mpz_null(mpz_t x) {
        x[0]._mp_alloc = 0;
        x[0]._mp_size = 0;
        x[0]._mp_d = nullptr;
    }
}