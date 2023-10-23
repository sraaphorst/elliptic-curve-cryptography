/**
 * big_int.h
 * By Sebastian Raaphorst, 2023.
 */

#include <iostream>
#include "big_int.h"
#include "modular_int.h"
#include "operations.h"
#include "printable.h"

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
    std::cout << "5 >= 3: " << (5 >= 3) << '\n';

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
}