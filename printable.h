/**
 * printable.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <ostream>
#include <string>

namespace ecc::printable {
    template <typename T>
    concept Printable = requires(const T& t) {
        { t.to_string() } -> std::same_as<std::string>;
    };

    template <Printable T>
    std::ostream &operator<<(std::ostream &out, const T &t) {
        return out << t.to_string();
    }
}