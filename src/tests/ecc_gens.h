/**
 * ecc_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#ifdef DEBUG
#include <iostream>
#endif

#include <rapidcheck.h>
#include <big_int.h>
#include <modular_int.h>
#include <vector>

#include "gmp_gens.h"

namespace rc {
    template<>
    struct Arbitrary<ecc::BigInt> {
        static Gen<ecc::BigInt> arbitrary() {
            return gen::map<gmp_mpz_t>([](gmp_mpz_t &&value) {
                return ecc::BigInt{value.value};
            });
        }
    };

    // Generate a probably prime gmp_mpz_t
    Gen<gmp_mpz_t> arbitraryPrimeGmp() {
        return gen::exec([]() {
            mpz_t random_num;
            mpz_init(random_num);

            // Generate a probably prime number
            do {
                mpz_urandomb(random_num, Arbitrary<gmp_mpz_t>::state.state, Arbitrary<gmp_mpz_t>::n);
            } while (!mpz_probab_prime_p(random_num, 25));

#ifdef DEBUG
            std::clog << "arbitrary prime: " << mpz_get_str(nullptr, 10, random_num) << '\n';
#endif
            gmp_mpz_t v{random_num};

            mpz_clear(random_num);

#ifdef DEBUG
            std::clog << "value now " << mpz_get_str(nullptr, 10, v.value) << '\n';
#endif
            return v;
        });
    }

    template<>
    struct Arbitrary<ecc::ModularInt> {
        static Gen<ecc::ModularInt> arbitrary() {
            return gen::exec([]() {
                const auto value = *gen::arbitrary<ecc::BigInt>();

                // To avoid overhead of copying extra objects, use the gmp prime generation.
                const ecc::BigInt mod{(*arbitraryPrimeGmp()).value};
#ifdef DEBUG
                const auto pp = mod.is_probably_prime(25);
                std::clog << "Picked " << value.to_string() << " and " << mod.to_string() << " with pp=" << pp << '\n';
#endif
                return ecc::ModularInt{value, mod};
            });
        }
    };

    // Generate a modular int that is a residue.
    const auto residueModularInt = gen::suchThat<ecc::ModularInt>([](const ecc::ModularInt &mi) {
        return mi.legendre() == ecc::ModularInt::Legendre::RESIDUE;
    });
}