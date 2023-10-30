/**
 * ecc_gens.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <rapidcheck.h>
#include <big_int.h>
#include <modular_int.h>

#include "gmp_gens.h"

namespace rc {
    template<>
    struct Arbitrary<ecc::BigInt> {
        static Gen<ecc::BigInt> arbitrary() {
            return gen::map(gen::arbitrary<gmp_mpz_t>(), [](const gmp_mpz_t& value) {
                return ecc::BigInt{value.value};
            });
        }
    };
}

namespace rc {
    template<>
    struct Arbitrary<ecc::ModularInt> {
        static Gen<ecc::ModularInt> arbitrary() {
            return gen::map(gen::arbitrary<std::tuple<ecc::BigInt, ecc::BigInt>>(),
                    [](const auto &vals) {
                        const auto &[value, mod] = vals;
                        return ecc::ModularInt{value, mod};
                    });
        }
    };
}