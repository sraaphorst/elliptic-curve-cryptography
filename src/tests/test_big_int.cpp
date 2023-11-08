/**
 * test_big_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <rapidcheck.h>
#include <big_int.h>
#include "ecc_gens.h"

using namespace ecc;

int main() {
    rc::check("test string_view constructor",
              [](const ecc::BigInt &bi) {
        const auto s = bi.to_string();
        const BigInt bi_s{s};
        RC_ASSERT(bi == bi_s);
    });

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