/**
 * point.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include "modular_int.h"

namespace ecc {
    class Point final {
    public:
        Point() = delete;
        Point(ModularInt x, ModularInt y);

        [[nodiscard]] inline const ModularInt &x() const noexcept {
            return _x;
        }
        [[nodiscard]] inline const ModularInt &y() const noexcept {
            return _y;
        }
    private:
        ModularInt _x, _y;

        // Check to see if the mod values are the same: if not, throw a domain_exception.
        static void check_same_mod(const ModularInt&, const ModularInt&);
    };
}

