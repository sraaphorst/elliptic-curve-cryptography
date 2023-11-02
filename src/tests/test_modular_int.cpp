/**
 * test_modular_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <rapidcheck.h>
#include <modular_int.h>
#include <printable.h>
#include "ecc_gens.h"
#include <signal.h>


using namespace ecc;
using namespace ecc::printable;

int main() {
//    rc::check("tst inverse",
//              [](const ModularInt &m) {
//                  std::clog << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime(25) << '\n';
//                  const auto inv_opt = m.invert();
//                  RC_PRE(inv_opt.has_value());
//                  const auto &inv = *inv_opt;
//                  std::clog << "Inverse " << inv << '\n';
//                  RC_ASSERT((m * inv).get_value() == 1);
//                  std::clog << "Done iteration\n\n\n";
//              });
//signal(SIGTRAP, [](int a) {
//    std::clog << "Here\n";
//    std::clog << "What happen?\n";
//});
    rc::check("test sqrt",
              [](const ModularInt &m) {
                  RC_PRE(m.residue());
                  // RC_PRE is broken here. If we call:
                  // RC_PRE(m.legendre() == ModularInt::Legendre::RESIDUE);
                  // then we get a SIGTRAP upon the first failure.
//                  ModularInt m = mc;
//                  while (m.legendre() != ModularInt::Legendre::RESIDUE)
//                      m = *rc::residueModularInt;
                  std::clog << m.to_string() << " has legendre " << ModularInt::legendre_value(m.legendre()) << '\n';
                  std::clog << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime() << '\n';
//                  RC_PRE(m.residue());
//                  RC_ASSERT(m.sqrt().has_value());
                  const auto sqrt = *(m.sqrt());
                  std::clog << "sqrt " << sqrt << '\n';
                  RC_ASSERT(sqrt * sqrt == m);
                  std::clog << "Done iteration\n\n\n";
              });
    return 0;
}
