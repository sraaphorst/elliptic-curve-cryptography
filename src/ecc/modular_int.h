/**
 * ModularInt.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <functional>
#include <optional>
#include <string_view>

#include "big_int.h"

namespace ecc {
    class ModularInt {
    public:
        enum class Legendre {
            DIVIDES = 0,
            NOT_RESIDUE = -1,
            RESIDUE = 1,
        };
        static int legendre_value(Legendre) noexcept;

        ModularInt() = delete;
        ModularInt(const ModularInt&) = default;
        ModularInt(ModularInt&&) noexcept = default;
        ModularInt(BigInt, BigInt);
        ModularInt(const std::string_view&);
        ~ModularInt() = default;

        ModularInt &operator=(const ModularInt&) = default;
        ModularInt &operator=(ModularInt&&) noexcept = default;

        [[nodiscard]] ModularInt operator-() const;
        [[nodiscard]] ModularInt operator+(const ModularInt&) const;
        [[nodiscard]] ModularInt operator-(const ModularInt&) const;
        [[nodiscard]] ModularInt operator*(const ModularInt&) const;
        [[nodiscard]] ModularInt operator/(const ModularInt&) const;
        [[nodiscard]] ModularInt pow(const BigInt&) const;
        [[nodiscard]] ModularInt pow(long) const;

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
            return _value;
        }

        [[nodiscard]] inline const BigInt &get_mod() const {
            return _mod;
        }

        [[nodiscard]] std::string to_string() const noexcept;

        // Find the Legendre _value of (_value/_mod), which is:
        // 0 if _mod | _value
        // 1 if _value is a residue class, i.e. there exists b such that b^2 ≡ _value (_mod)
        // -1 otherwise.
        [[nodiscard]] Legendre legendre() const;
        [[nodiscard]] bool residue() const;

        // Return the square root of the number if it exists, and std::nullopt.
        [[nodiscard]] std::optional<ModularInt> sqrt() const;

        // Find the multiplicative inverse of this element if it exists, which
        // is the case iff gcd(_value, _mod) == 1.
        [[nodiscard]] std::optional<ModularInt> invert() const;

        [[nodiscard]] const inline BigInt &value() const noexcept {
            return _value;
        }
        [[nodiscard]] const inline BigInt &mod() const noexcept {
            return _mod;
        }

    private:
        BigInt _value;
        BigInt _mod;

        // Delegates to pair-extracted constructor.
        explicit ModularInt(std::pair<BigInt, BigInt>&&);

        using bigint_func1 = std::function<BigInt(const BigInt&)>;
        using bigint_func2 = std::function<BigInt(const BigInt&, const BigInt&)>;

        // Check to see if the _mod values are the same: if not, throw a domain_exception.
        void check_same_mod(const ModularInt&) const;

        [[nodiscard]] ModularInt op(const bigint_func1&) const;
        [[nodiscard]] ModularInt op(const bigint_func2&, const ModularInt&) const;
        [[nodiscard]] ModularInt &op_set(const bigint_func2&, const ModularInt&);
    };
}
