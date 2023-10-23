/**
 * ModularInt.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include "modular_int.h"

#include <utility>

namespace ecc {
    // Make sure we reduce in case value >= mod.
    ModularInt::ModularInt(BigInt value, BigInt mod): value{std::move(value)}, mod{std::move(mod)} {
        this->value %= this->mod;
    }
}
