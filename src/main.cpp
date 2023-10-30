/**
 * big_int.h
 * By Sebastian Raaphorst, 2023.
 */

#include <iostream>
#include "ecc/big_int.h"
#include "ecc/modular_int.h"
#include "ecc/operations.h"
#include "ecc/printable.h"

using namespace ecc;
using namespace ecc::printable;
using namespace ecc::operations;

int main() {
    BigInt a(255);
    BigInt b("24386246891236498216984621934812");
    BigInt c("4382957847829743892748927432");
    BigInt d = (-a * b) % c + 3;
    std::cout << '(' << -a << " * " << b << ") % " << c << " + 3 = " << d << '\n';

    // This uses == and < on BigInt to define >=.
    std::cout << b << " >= " << c << ": " << (b >= c) << '\n';

    // Just to show operations do not cause a clash.
    std::cout << "5 >= 3: " << (5 >= 3) << "\n\n";

    // Extended GCD.
    const BigInt g1{2 * 3 * 3 * 5 * 7 * 11 * 13 * 19 * 19 * 19};
    const BigInt g2{3 * 5 * 7 * 7 * 13 * 19 * 19 * 23 * 23};
    const BigInt expected{3 * 5 * 7 * 13 * 19 * 19};
    BigInt x1, x2;
    const auto gcd1 = g1.extended_gcd(g2, x1, x2);
    const auto gcd2 = g1.gcd(g2);
    std::cout << "gcd(" << g1 << ", " << g2 << ") = " << gcd1 << ", expected = " << expected << '\n';
    std::cout << gcd2 << " = " << x1 << " * " << g1 << " + " << x2 << " * " << g2 << " -> " << (x1 * g1 + x2 * g2) << "\n\n";


    // Let's do a test mod 7.
    constexpr const auto n = 7;
    for (long i=0; i < n; ++i) {
        const ModularInt mi(i, 7);
        std::cout << "*** " << mi << " ***\n";
        const auto nmi = -mi;
        std::cout << '-' << mi << " = " << nmi << ", sum: " << (mi + nmi) << '\n';
        std::cout << mi << " * " << nmi << " = " << (mi * nmi) << '\n';
        for (long j=i; j < n; ++j) {
            const ModularInt mj(j, 7);
            std::cout << mj << ":\n";
            std::cout << "== " << (mi == mj) << ", != " << (mi != mj)
                      << ", < " << (mi < mj) << ", <= " << (mi <= mj)
                      << ", > " << (mi > mj) << ", >= " << (mi >= mj) << '\n';
        }
    }

    std::cout << "\n\nINVERSE:\n";
    const ModularInt inv{BigInt{"732874823"}, BigInt{"32918938193048321"}};
    const auto inv_calc = inv.invert();
    if (inv_calc.has_value())
        std::cout << "The inverse of " << inv << " is " << inv_calc.value() << ": " << (inv * inv_calc.value()) << "\n";
    else
        std::cout << "ERROR: " << inv << " was determined to have no inverse.\n";

    // Primality checking.
    const BigInt prime{"864211111111111111111111"};
    if (prime.is_probably_prime())
        std::cout << prime << " is (probably) prime.\n";
    else
        std::cout << prime << " is not prime.\n";

    const BigInt comp{"521649490603448721200401"};
    if (comp.is_probably_prime())
        std::cout << comp << " is (probably) prime.\n";
    else
        std::cout << comp << " is not prime.\n";

    std::cout << "\n\nLEGENDRE:\n";
    for (long i=0; i < 3; ++i) {
        ModularInt legtest{i, 3};
        std::cout << legtest << ": " << ModularInt::legendre_value(legtest.legendre()) << '\n';
    }

//    for (int i=2; i < 12161; ++i) {
//        const auto m = ModularInt{i, 12161};
//        m.sqrt();
//        std::cout << i << '\n';
//    }

    const auto m = ModularInt{56, 129};
    const auto result = m.pow(4);
    std::cout << m << "^4 = " << result << '\n';
}