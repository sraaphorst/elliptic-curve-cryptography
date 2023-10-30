/**
 * gmp_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <iostream>
#include <chrono>
#include <rapidcheck.h>
#include <gmp.h>

// We want to wrap an mpz_t in a struct to automatically dealloc it when it goes out of scope.
struct gmp_mpz_t {
    mpz_t value;

    gmp_mpz_t() {
        std::cout << "in default\n";
        mpz_init(value);
    }

    gmp_mpz_t(const mpz_t& gmp) {
        mpz_init_set(value, gmp);
        std::cout << "in const mpz_t&: " << mpz_get_str(nullptr, 10, gmp)
                  << ", value: " << mpz_get_str(nullptr, 10, gmp) << "\n";
    }

    gmp_mpz_t(const gmp_mpz_t& other) {
        mpz_init_set(value, other.value);
        std::cout << "in const gmp_mpz_t&: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << "\n";
    }

    ~gmp_mpz_t() {
        mpz_clear(value);
    }

    gmp_mpz_t& operator=(const gmp_mpz_t& other) {
        std::cout << "in =: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << "\n";
        mpz_set(value, other.value);
        std::cout << "now: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << "\n";
        return *this;
    }

    [[nodiscard]] bool operator==(const gmp_mpz_t &other) const {
        return mpz_cmp(value, other.value) == 0;
    }

    [[nodiscard]] bool operator!=(const gmp_mpz_t &other) const {
        return !(*this == other);
    }
};

struct RandomState {
    gmp_randstate_t state;

    RandomState() {
        gmp_randinit_mt(state);
        const auto time = std::chrono::high_resolution_clock::now();
        const auto duration = time.time_since_epoch();
        const auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        gmp_randseed_ui(state, timestamp);
    }

    ~RandomState() {
        gmp_randclear(state);
    }
};

namespace rc {
    template<>
    struct Arbitrary<gmp_mpz_t> {
        const static mp_bitcnt_t n = 64;
        static RandomState state;

        Arbitrary() {
            std::cout << "Initializing Arbitrary<gmp_mpz_t>\n";
        }

        ~Arbitrary() {
            std::cout << "Cleared Arbitrary<gmp_mpz_t>\n";
        }

        static Gen<gmp_mpz_t> arbitrary() {
            return gen::exec([]() {
                mpz_t random_num;
                mpz_init(random_num);
                mpz_urandomb(random_num, state.state, n);

                std::cout << "generated random: " << mpz_get_str(nullptr, 10, random_num) << '\n';
                gmp_mpz_t v{random_num};

                mpz_clear(random_num);

                std::cout << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
                return v;
            });
        }
    };

    RandomState Arbitrary<gmp_mpz_t, void>::state;
}
