/**
 * big_int.h
 * By Sebastian Raaphorst, 2023.
 */

#include <iostream>
#include "ecc/big_int.h"
#include "ecc/modular_int.h"
#include "ecc/operations.h"
#include "ecc/printable.h"
#include <gmpxx.h>

using namespace ecc;
using namespace ecc::printable;
using namespace ecc::operations;

void check_same_mod(const ModularInt &m1, const ModularInt &m2) {
    if (m1.get_mod() != m2.get_mod()) {
//        throw std::domain_error(m1.to_string());
        throw std::domain_error("Argh");
    }
}

void power(std::string &&as, std::string &&bs, std::string &&ps) {
    mpz_class a(as);
    mpz_class b(bs);
    mpz_class p(ps);

    mpz_class r;
    mpz_powm(r.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t(), p.get_mpz_t());

    std::cout << "Result: " << r.get_str() << std::endl;
}

int main() {
//    BigInt a(255);
//    BigInt b("24386246891236498216984621934812");
//    BigInt c("4382957847829743892748927432");
//    BigInt d = (-a * b) % c + 3;
//
//    mpz_class ma(255);
//    mpz_class mb("24386246891236498216984621934812");
//    mpz_class mc("4382957847829743892748927432");
//    mpz_class md = (-ma * mb) % mc + 3;
//    mpz_class mma = -ma;
//    const auto s = static_cast<mpz_class>(mma).get_str();
//    std::cout << '(' <<  mma.get_str() << " * " << mb.get_str() << ") % " << mc.get_str() << " + 3 = " << md.get_str() << '\n';
//
//    // This uses == and < on BigInt to define >=.
//    std::cout << b << " >= " << c << ": " << (b >= c) << '\n';
//
//    // Just to show operations do not cause a clash.
//    std::cout << "5 >= 3: " << (5 >= 3) << "\n\n";
//
//    // Extended GCD.
//    const BigInt g1{2 * 3 * 3 * 5 * 7 * 11 * 13 * 19 * 19 * 19};
//    const BigInt g2{3 * 5 * 7 * 7 * 13 * 19 * 19 * 23 * 23};
//    const BigInt expected{3 * 5 * 7 * 13 * 19 * 19};
//    BigInt x1, x2;
//    const auto gcd1 = g1.extended_gcd(g2, x1, x2);
//    const auto gcd2 = g1.gcd(g2);
//    std::cout << "gcd(" << g1 << ", " << g2 << ") = " << gcd1 << ", expected = " << expected << '\n';
//    std::cout << gcd2 << " = " << x1 << " * " << g1 << " + " << x2 << " * " << g2 << " -> " << (x1 * g1 + x2 * g2) << "\n\n";
//
//
//    // Let's do a test _mod 7.
//    constexpr const auto n = 7;
//    for (long i=0; i < n; ++i) {
//        const ModularInt mi(i, 7);
//        std::cout << "*** " << mi << " ***\n";
//        const auto nmi = -mi;
//        std::cout << '-' << mi << " = " << nmi << ", sum: " << (mi + nmi) << '\n';
//        std::cout << mi << " * " << nmi << " = " << (mi * nmi) << '\n';
//        for (long j=i; j < n; ++j) {
//            const ModularInt mj(j, 7);
//            std::cout << mj << ":\n";
//            std::cout << "== " << (mi == mj) << ", != " << (mi != mj)
//                      << ", < " << (mi < mj) << ", <= " << (mi <= mj)
//                      << ", > " << (mi > mj) << ", >= " << (mi >= mj) << '\n';
//        }
//    }
//
//    std::cout << "\n\nINVERSE:\n";
//    const ModularInt inv{BigInt{"732874823"}, BigInt{"32918938193048321"}};
//    const auto inv_calc = inv.invert();
//    if (inv_calc.has_value())
//        std::cout << "The inverse of " << inv << " is " << inv_calc._value() << ": " << (inv * inv_calc._value()) << "\n";
//    else
//        std::cout << "ERROR: " << inv << " was determined to have no inverse.\n";
//
//    // Primality checking.
//    const BigInt prime{"864211111111111111111111"};
//    if (prime.is_probably_prime())
//        std::cout << prime << " is (probably) prime.\n";
//    else
//        std::cout << prime << " is not prime.\n";
//
//    const BigInt comp{"521649490603448721200401"};
//    if (comp.is_probably_prime())
//        std::cout << comp << " is (probably) prime.\n";
//    else
//        std::cout << comp << " is not prime.\n";
//
//    std::cout << "\n\nLEGENDRE:\n";
//    for (long i=0; i < 3; ++i) {
//        ModularInt legtest{i, 3};
//        std::cout << legtest << ": " << ModularInt::legendre_value(legtest.legendre()) << '\n';
//    }
//
////    for (int i=2; i < 12161; ++i) {
////        const auto m = ModularInt{i, 12161};
////        m.sqrt();
////        std::cout << i << '\n';
////    }
//
//    const auto m = ModularInt{56, 129};
//    const auto result = m.pow(4);
//    std::cout << m << "^4 = " << result << '\n';
//
//    power("272076278251345488", "716710961654983669", "2866843846619934677");

    BigInt v1{"410468015445649"};
    BigInt m1{"755255173926857"};
    ModularInt mi{v1, m1};
    const auto sq = mi.sqrt();
    if (sq.has_value())
        std::cout << "sqrt is: " << *sq << '\n';
    else
        std::cout << "no value\n";
//    try {
//        BigInt b1v{1};
//        BigInt b1m{3};
//        BigInt b2v{1};
//        BigInt b2m{5};
//        ModularInt mi1{b1v, b1m};
//        ModularInt mi2{b2v, b2m};
//        check_same_mod(mi1, mi2);
//    } catch (std::domain_error &ex) {
//        std::cout << "Domain error: " << ex.what() << '\n';
//    } catch (...) {
//        std::cout << "Other weirdness.\n";
//    }
}