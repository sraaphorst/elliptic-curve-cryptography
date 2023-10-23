/**
 * ModularInt.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include "modular_int.h"

#include <format>
#include <stdexcept>

namespace ecc {
    // Make sure we reduce in case value >= mod.
    ModularInt::ModularInt(BigInt value, BigInt mod): value{std::move(value)}, mod{std::move(mod)} {
        if (this->mod.zero()) {
            throw std::domain_error(std::format("Error: {} mod 0.", this->value.to_string()));
        }
        this->value %= this->mod;
    }
}
