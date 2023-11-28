/**
 * gmp_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#ifdef DEBUG
#include <iostream>
#endif

#include <chrono>
#include <rapidcheck.h>
#include <gmp.h>


// We want to wrap an mpz_t in a struct to automatically dealloc it when it goes out of scope.
struct gmp_mpz_t {
    mpz_t value;

    gmp_mpz_t() {
#ifdef DEBUG
        std::clog << "gmp_mpt_t default\n";
#endif
        mpz_init(value);
    }

    gmp_mpz_t(const mpz_t &gmp) {
        mpz_init_set(value, gmp);
#ifdef DEBUG
        std::clog << "gmp_mpz_t const mpz_t&: " << mpz_get_str(nullptr, 10, gmp)
                  << ", _value: " << mpz_get_str(nullptr, 10, gmp) << '\n';
#endif
    }

    gmp_mpz_t(const gmp_mpz_t &other) {
        mpz_init_set(value, other.value);
#ifdef DEBUG
        std::clog << "gmp_mpz_t const gmp_mpz_t&: " << mpz_get_str(nullptr, 10, other._value)
                  << ", _value: " << mpz_get_str(nullptr, 10, _value) << '\n';
#endif
    }

    gmp_mpz_t(gmp_mpz_t &&other) {
        mpz_swap(value, other.value);
        // This is needed and is the convention in mpz_class.
        mpz_init(other.value);
#ifdef DEBUG
        std::clog << "gmp_mpz_t &&: " << mpz_get_str(nullptr, 10, other._value) << '\n';
#endif
    }

    ~gmp_mpz_t() {
        mpz_clear(value);
    }

    gmp_mpz_t &operator=(const gmp_mpz_t &other) {
#ifdef DEBUG
        std::clog << "gmp_mpz_t =: " << mpz_get_str(nullptr, 10, other._value)
                  << ", _value: " << mpz_get_str(nullptr, 10, _value) << "\n";
#endif
        if (mpz_cmp(value, other.value))
            mpz_set(value, other.value);
#ifdef DEBUG
        std::clog << "now: " << mpz_get_str(nullptr, 10, other._value)
                  << ", _value: " << mpz_get_str(nullptr, 10, _value) << "\n";
#endif
        return *this;
    }

    gmp_mpz_t &operator=(gmp_mpz_t &&other) noexcept {
#ifdef DEBUG
            std::clog << "gmp_mpz_t &&=: " << mpz_get_str(nullptr, 10, other._value)
                      << ", _value: " << mpz_get_str(nullptr, 10, _value) << "\n";
#endif
        std::swap(*value, *other.value);
        return *this;
    }

    [[nodiscard]] bool operator==(const gmp_mpz_t &other) const {
        return mpz_cmp(value, other.value) == 0;
    }

    [[nodiscard]] bool operator<(const gmp_mpz_t &other) const {
        return mpz_cmp(value, other.value) < 0;
    }

    [[nodiscard]] std::string to_string() const {
        return mpz_get_str(nullptr, 10, value);
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
#ifdef DEBUG
            std::clog << "Initializing Arbitrary<gmp_mpz_t>\n";
#endif
        }

        ~Arbitrary() {
#ifdef DEBUG
            std::clog << "Cleared Arbitrary<gmp_mpz_t>\n";
#endif
        }

        static Gen<gmp_mpz_t> arbitrary() {
            return gen::exec([]() {
                mpz_t random_num;
                mpz_init(random_num);
                mpz_urandomb(random_num, state.state, n);

#ifdef DEBUG
                std::clog << "gmp_mpz_t arbitrary: " << mpz_get_str(nullptr, 10, random_num) << '\n';
#endif
                gmp_mpz_t v{random_num};
                mpz_clear(random_num);

#ifdef DEBUG
                std::clog << "_value now " << mpz_get_str(nullptr, 10, v._value) << '\n';
#endif
                return v;
            });
        }
    };

    RandomState Arbitrary<gmp_mpz_t, void>::state;
}
