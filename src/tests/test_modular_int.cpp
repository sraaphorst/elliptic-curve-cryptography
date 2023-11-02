/**
 * test_modular_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#ifdef DEBUG
#include <iostream>
#include <printable.h>
#endif

#include <rapidcheck.h>
#include <modular_int.h>
#include "ecc_gens.h"


using namespace ecc;

#ifdef DEBUG
using namespace ecc::printable;
#endif

int main() {
    rc::check("tst inverse",
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
              [](const ModularInt &mc) {
                  // This works just fine on Ubuntu, but not on Mac OS X 14 for some reason.
                  // It causes a SIGTRAP with exit code 133.
#ifndef __APPLE__
                  RC_PRE(mc.residue());
                  ModularInt m = mc;
#else
                  ModularInt m = mc;
                  while (m.legendre() != ModularInt::Legendre::RESIDUE)
                      m = *rc::residueModularInt;
#endif
#ifdef DEBUG
                  std::clog << m.to_string() << " has legendre " << ModularInt::legendre_value(m.legendre()) << '\n';
                  std::clog << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime() << '\n';
#endif
                  RC_ASSERT(m.sqrt().has_value());
                  const auto sqrt = *(m.sqrt());
#ifdef DEBUG
                  std::clog << "sqrt " << sqrt << '\n';
#endif
                  RC_ASSERT(sqrt * sqrt == m);
#ifdef DEBUG
                  std::clog << "Done iteration\n\n\n";
#endif
              });
    return 0;
}
