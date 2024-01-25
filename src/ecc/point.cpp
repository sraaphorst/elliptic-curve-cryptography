/**
 * point.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <fmt/core.h>
#include <fmt/format.h>

#include "point.h"

namespace ecc {
    Point::Point(ecc::ModularInt x, ecc::ModularInt y): _x{std::move(x)}, _y{std::move(y)} {

    }

    void Point::check_same_mod(const ecc::ModularInt &x, const ecc::ModularInt &y) {
        if (x.get_mod() != y.get_mod())

    }
}