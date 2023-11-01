/**
 * test_modular_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <rapidcheck.h>
#include <modular_int.h>
#include <printable.h>
#include "ecc_gens.h"


using namespace ecc;
using namespace ecc::printable;

int main() {
    rc::check("tst inverse",
              [](const ModularInt &m) {
                    std::clog << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime(25) << '\n';
                    const auto inv_opt = m.invert();
                    std::clog << "Checking for inverse of " << m << '\n';
                    if (inv_opt.has_value()) {
                        const auto &inv = *inv_opt;
                        std::clog << "Inverse " << inv << '\n';
                        RC_ASSERT((m * inv).get_value() == 1);
                        std::clog << "Done iteration\n\n\n";
                    }
              });
    rc::check("test sqrt",
              [](const ModularInt &m) {
                    std::clog << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime() << '\n';
                    const auto sqrt_opt = m.sqrt();
                    std::clog << "Checking for sqrt of " << m << '\n';
                    if (sqrt_opt.has_value()) {
                        const auto &sqrt = *sqrt_opt;
                        std::clog << "sqrt " << sqrt << '\n';
                        RC_ASSERT(sqrt * sqrt == m);
                    } else {
                        std::clog << "No sqrt, legendre=" << ModularInt::legendre_value(m.legendre()) << '\n';
                    }
                  std::clog << "Done iteration\n\n\n";
              });
    return 0;
}
