/**
 * gmp_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <iostream>
#include <chrono>
#include <rapidcheck.h>
#include <gmp.h>

#include <gmp_ops.h>

// We want to wrap an mpz_t in a struct to automatically dealloc it when it goes out of scope.
struct gmp_mpz_t {
    mpz_t value;

    gmp_mpz_t() {
        std::cerr << "in default\n";
        mpz_init(value);
    }

    gmp_mpz_t(const mpz_t& gmp) {
        mpz_init_set(value, gmp);
        std::cerr << "in const mpz_t&: " << mpz_get_str(nullptr, 10, gmp)
                  << ", value: " << mpz_get_str(nullptr, 10, gmp) << '\n';
    }

    gmp_mpz_t(const gmp_mpz_t& other) {
        mpz_init_set(value, other.value);
        std::cerr << "in const gmp_mpz_t&: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << '\n';
    }

    gmp_mpz_t(gmp_mpz_t &&other) {
        std::cerr << "gmp_mpz_t &&: " << mpz_get_str(nullptr, 10, other.value) << '\n';
        ecc::gmp_ops::mpz_move(value, other.value);
        ecc::gmp_ops::mpz_null(other.value);
    }

    ~gmp_mpz_t() {
        mpz_clear(value);
    }

    gmp_mpz_t& operator=(const gmp_mpz_t& other) {
        std::cerr << "in =: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << "\n";
        if (mpz_cmp(value, other.value))
            mpz_set(value, other.value);
        std::cerr << "now: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << "\n";
        return *this;
    }

    gmp_mpz_t &operator=(gmp_mpz_t &&other) noexcept {
        std::cerr << "in &&=: " << mpz_get_str(nullptr, 10, other.value)
                  << ", value: " << mpz_get_str(nullptr, 10, value) << "\n";
        mpz_swap(value, other.value);
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
            std::cerr << "Initializing Arbitrary<gmp_mpz_t>\n";
        }

        ~Arbitrary() {
            std::cerr << "Cleared Arbitrary<gmp_mpz_t>\n";
        }

        static Gen<gmp_mpz_t> arbitrary() {
            return gen::exec([]() {
                mpz_t random_num;
                mpz_init(random_num);
                mpz_urandomb(random_num, state.state, n);

                std::cerr << "generated random: " << mpz_get_str(nullptr, 10, random_num) << '\n';
                gmp_mpz_t v{random_num};

                mpz_clear(random_num);

                std::cerr << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
                return v;
            });
        }
    };

    RandomState Arbitrary<gmp_mpz_t, void>::state;

    const auto ProbablyPrimeGmp = gen::suchThat<gmp_mpz_t>([] (gmp_mpz_t &&g) {
        const auto p = mpz_probab_prime_p(g.value, 1);
        if (p)
            std::cerr << "Found probably prime value " << mpz_get_str(nullptr, 10, g.value) << '\n';
        return mpz_probab_prime_p(g.value, 1);
    });
}
