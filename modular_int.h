/**
 * ModularInt.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include "big_int.h"

namespace ecc {
    class ModularInt {
    private:
        BigInt value;
        BigInt mod;

    public:
        ModularInt(BigInt value, BigInt mod);
        ModularInt(const ModularInt&) = default;
        ModularInt(ModularInt&&) noexcept = default;


    };
}