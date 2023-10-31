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
                    std::cerr << "Received: " << m.to_string() << ", pp: " << m.get_mod().is_probably_prime() << '\n';
                    const auto inv_opt = m.invert();
                    std::cerr << "Checking for inverse of " << m << '\n';
                    if (inv_opt.has_value()) {
                        const auto &inv = *inv_opt;
                        std::cerr << "Inverse " << inv << '\n';
                        RC_ASSERT((m * inv).get_value() == 1);
                        std::cerr << "Done iteration\n\n\n";
                    }
              });
//    rc::check("test sqrt",
//              [](const ModularInt &m) {
//                    const auto sqrt_opt = m.sqrt();
//                    std::cout << "Checking for sqrt of " << m << '\n';
//                    if (sqrt_opt.has_value()) {
//                        const auto &sqrt = *sqrt_opt;
//                        std::cout << "sqrt " << sqrt << '\n';
//                        RC_ASSERT(sqrt * sqrt == m);
//                    }
//              });
    return 0;
}