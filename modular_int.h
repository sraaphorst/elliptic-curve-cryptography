/**
 * ModularInt.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <functional>
#include <optional>
#include <ostream>

#include "big_int.h"

namespace ecc {
    class ModularInt {
    public:
        ModularInt(BigInt value, BigInt mod);
        ModularInt(const ModularInt&) = default;
        ModularInt(ModularInt&&) noexcept = default;

        ModularInt &operator=(const ModularInt&) = default;
        ModularInt &operator=(ModularInt&&) noexcept = default;

        [[nodiscard]] ModularInt operator-() const;
        [[nodiscard]] ModularInt operator+(const ModularInt&) const;
        [[nodiscard]] ModularInt operator-(const ModularInt&) const;
        [[nodiscard]] ModularInt operator*(const ModularInt&) const;
        [[nodiscard]] ModularInt operator/(const ModularInt&) const;

        ModularInt &operator+=(const ModularInt&);
        ModularInt &operator-=(const ModularInt&);
        ModularInt &operator*=(const ModularInt&);
        ModularInt &operator/=(const ModularInt&);

        ModularInt &operator++();
        ModularInt operator++(int);

        ModularInt &operator--();
        ModularInt operator--(int);

        // Note that this defines a PARTIAL ordering and not a total ordering.
        [[nodiscard]] bool operator==(const ModularInt&) const;
        [[nodiscard]] bool operator<(const ModularInt&) const;

        [[nodiscard]] inline const BigInt &get_value() const {
            return value;
        }

        [[nodiscard]] inline const BigInt &get_mod() const {
            return mod;
        }

        [[nodiscard]] std::string to_string() const noexcept;

        // Find the multiplicative inverse of this element if it exists, which
        // is the case iff gcd(value, mod) == 1.
        [[nodiscard]] std::optional<ModularInt> invert() const;

    private:
        BigInt value;
        BigInt mod;

        using bigint_func1 = std::function<BigInt(const BigInt&)>;
        using bigint_func2 = std::function<BigInt(const BigInt&, const BigInt&)>;
        using compare_bigint = std::function<bool(const BigInt&, const BigInt&)>;

        // Check to see if the mod values are the same: if not, throw a domain_exception.
        void check_same_mod(const ModularInt&) const;

        [[nodiscard]] ModularInt op(const bigint_func1&) const;
        [[nodiscard]] ModularInt op(const bigint_func2&, const ModularInt&) const;
        [[nodiscard]] ModularInt &op_set(const bigint_func2&, const ModularInt&);
    };
}
