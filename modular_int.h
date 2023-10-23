/**
 * ModularInt.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <functional>
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
        ModularInt &operator++(int);

        ModularInt &operator--();
        ModularInt &operator--(int);

        [[nodiscard]] bool operator==(const ModularInt&) const;
        [[nodiscard]] bool operator!=(const ModularInt&) const;
        [[nodiscard]] bool operator<(const ModularInt&) const;
        [[nodiscard]] bool operator<=(const ModularInt&) const;
        [[nodiscard]] bool operator>(const ModularInt&) const;
        [[nodiscard]] bool operator>=(const ModularInt&) const;

    private:
        BigInt value;
        BigInt mod;

        using bigint_func1 = std::function<BigInt(const BigInt&)>;
        using bigint_func2 = std::function<BigInt(const BigInt&, const BigInt&)>;

        // Check to see if the mod values are the same: if not, throw a domain_exception.
        void check_same_mod(const ModularInt&) const;

        [[nodiscard]] ModularInt op(const bigint_func1&) const;
        [[nodiscard]] ModularInt op(const bigint_func2&, const ModularInt&) const;
        [[nodiscard]] ModularInt &op_set(const bigint_func2&, const BigInt&);
    };
}

std::ostream &operator<<(std::ostream&, const ecc::ModularInt&);