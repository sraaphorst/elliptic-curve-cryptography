/**
 * test_modular_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <iostream>
#ifdef DEBUG
#include <iostream>
#include <printable.h>
#endif

#include <rapidcheck.h>
#include <operations.h>
#include <modular_int.h>
#include "ecc_gens.h"


using namespace ecc;
using namespace ecc::operations;

#ifdef DEBUG
using namespace ecc::printable;
#endif


int main() {
    rc::check("test string_view constructor",
              [](const ModularInt &m) {
        const auto s = m.to_string();
        const ModularInt m_s{s};
        RC_ASSERT(m == m_s);
    });

    rc::check("test inverse",
              [](const ModularInt &m) {
#ifdef DEBUG
                  std::clog << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime(25) << '\n';
#endif
                  const auto inv_opt = m.invert();
                  RC_PRE(inv_opt.has_value());
                  const auto &inv = *inv_opt;
#ifdef DEBUG
                  std::clog << "Inverse " << inv << '\n';
#endif
                  RC_ASSERT((m * inv).get_value() == 1);
#ifdef DEBUG
                  std::clog << "Done iteration\n\n\n";
#endif
              });

    rc::check("test sqrt",
              [](const ModularInt &m) {
                  RC_PRE(m.residue());
#ifdef DEBUG
                  std::clog << m << " has legendre " << ModularInt::legendre_value(m.legendre()) << '\n';
                  std::clog << "Received: " << m << ", probably prime: " << m.get_mod().is_probably_prime() << '\n';
#endif
                  const auto sqrt_opt = m.sqrt();
                  RC_ASSERT(sqrt_opt.has_value());
                  const auto &sqrt = *sqrt_opt;
#ifdef DEBUG
                  std::clog << "sqrt " << sqrt << '\n';
#endif
                  RC_ASSERT(sqrt * sqrt == m);
#ifdef DEBUG
                  std::clog << "Done iteration\n\n\n";
#endif
              });

    rc::check("non-compatible mod test",
              [](const ModularInt &m1, const ModularInt &m2) {
       RC_PRE(m1.get_mod() != m2.get_mod());
       RC_ASSERT_THROWS_AS((void)(m1 + m2), std::domain_error);
    });
}
