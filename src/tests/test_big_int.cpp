/**
 * test_bigint.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <rapidcheck.h>
#include <big_int.h>
#include <printable.h>
#include "gmp_gens.h"

using namespace ecc;
using namespace ecc::printable;

namespace rc {
    template<>
    struct Arbitrary<BigInt> {
        static Gen<BigInt> arbitrary() {
            return gen::map(gen::arbitrary<gmp_mpz_t>(), [](const gmp_mpz_t& value) {
               return BigInt{value.value};
            });
        }
    };
}

int main() {
    rc::check("test extended gcd",
              [](const std::tuple<BigInt, BigInt> &big_ints) {
        const auto &[bi1, bi2] = big_ints;
        BigInt coeff1, coeff2;
        const auto gcd = bi1.extended_gcd(bi2, coeff1, coeff2);

        // Make sure the GCD divides both bi1 and bi2.
        RC_ASSERT((bi1 % gcd).zero());
        RC_ASSERT((bi2 % gcd).zero());

        // Make sure the coefficients are correctly calculated.
        RC_ASSERT(coeff1 * bi1 + coeff2 * bi2 == gcd);
    });

    return 0;
}