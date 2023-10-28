/**
 * gmp_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <chrono>
#include <rapidcheck.h>
#include <gmp.h>

// We want to wrap an mpz_t in a struct to automatically dealloc it when it goes out of scope.
struct gmp_mpz_t {
    mpz_t value;

    gmp_mpz_t() {
        mpz_init(value);
    }

    gmp_mpz_t(const mpz_t& other) {
        mpz_init_set(value, other);
    }

    gmp_mpz_t(const gmp_mpz_t& other) {
        mpz_init_set(value, other.value);
    }

    ~gmp_mpz_t() {
        mpz_clear(value);
    }

    gmp_mpz_t& operator=(const gmp_mpz_t& other) {
        if (this != &other) {
            mpz_set(value, other.value);
        }
        return *this;
    }

    [[nodiscard]] bool operator==(const gmp_mpz_t &other) const {
        return mpz_cmp(value, other.value) == 0;
    }

    [[nodiscard]] bool operator!=(const gmp_mpz_t &other) const {
        return !(*this == other);
    }
};

namespace rc {
    template<>
    struct Arbitrary<gmp_mpz_t> {
        const static mp_bitcnt_t n = 64;

        static Gen<gmp_mpz_t> arbitrary() {
            return gen::exec([]() {
                gmp_randstate_t state;
                gmp_randinit_mt(state);

                const auto time = std::chrono::high_resolution_clock::now();
                const auto duration = time.time_since_epoch();
                const auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

                gmp_randseed_ui(state, timestamp);
                mpz_t random_num;
                mpz_init(random_num);
                mpz_urandomb(random_num, state, n);

                gmp_mpz_t v{random_num};

                mpz_clear(random_num);
                gmp_randclear(state);
                return v;
            });
        }
    };
}
