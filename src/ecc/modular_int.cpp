/**
 * ModularInt.cpp
 * By Sebastian Raaphorst, 2023.
 */

#ifdef DEBUG
#include <iostream>
#endif
#include <optional>
#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>

#include <fmt/core.h>
#include <fmt/format.h>
#include <gmp.h>

#include "operations.h"
#include "gmp_rng.h"

#include "formatters/big_int_formatter.h"
#include "formatters/modular_int_formatter.h"
#include "modular_int.h"

namespace ecc {
    using namespace operations;

    static gmp::gmp_rng rng;

    // The representation of a ModularInt.
    static std::string modular_int_string(const BigInt &value, const BigInt &mod) {
        return fmt::format("{}({})", value, mod);
    }

    int ModularInt::legendre_value(Legendre legendre) noexcept {
        return static_cast<int>(legendre);
    }

    // Used by the std::string_view constructor to extract the BigInts.
    [[nodiscard]] static std::pair<BigInt, BigInt> parse_big_ints(const std::string_view input_view) {
        const auto openPos = input_view.find('(');
        const auto closePos = input_view.find(')');

        if (openPos == std::string_view::npos
            || closePos == std::string_view::npos
            || closePos != input_view.length() - 1)
            throw std::domain_error(fmt::format("Not a valid ModularInt: {}", input_view));

        BigInt value{input_view.substr(0, openPos)};
        BigInt mod{input_view.substr(openPos + 1, closePos - openPos - 1)};
        return {std::move(value), std::move(mod)};
    }

    // Make sure we reduce in case _value >= _mod.
    ModularInt::ModularInt(BigInt value, BigInt mod): _value{std::move(value)}, _mod{std::move(mod)} {
        if (_mod.zero())
            throw std::domain_error(fmt::format("Tried to create a ModularInt with _mod 0: {}",
                                                modular_int_string(_value, _mod)));
        _value %= _mod;
    }

    ModularInt::ModularInt(const std::string_view &input_view):
        ModularInt(std::forward<std::pair<BigInt, BigInt>>(parse_big_ints(input_view))) {}

    ModularInt::ModularInt(std::pair<BigInt, BigInt> &&pair): ModularInt(std::get<0>(pair), std::get<1>(pair)) {}

    ModularInt ModularInt::operator-() const {
        return op(negation<BigInt>);
    }

    ModularInt ModularInt::operator+(const ModularInt &other) const {
        return op(addition<BigInt>, other);
    }

    ModularInt ModularInt::operator-(const ModularInt &other) const {
        return op(subtraction<BigInt>, other);
    }

    ModularInt ModularInt::operator*(const ModularInt &other) const {
        return op(multiplication<BigInt>, other);
    }

    ModularInt ModularInt::operator/(const ModularInt &other) const {
        return op(division<BigInt>, other);
    }

    ModularInt ModularInt::pow(const BigInt &n) const {
        mpz_t pvalue;
        mpz_init_set(pvalue, _value.value);
        mpz_powm(pvalue, pvalue, n.value, _mod.value);
        ModularInt result{pvalue, _mod};
        mpz_clear(pvalue);
        return result;
    }

   ModularInt ModularInt::pow(long n) const {
        ModularInt a{1, _mod};

        // Power 0 obviously gives 1 (_mod m).
        if (n == 0L)
            return a;

        // To take a^n, take {a^{-1}}^n.
        if (n < 0) {
            auto a_opt = invert();
            if (!a_opt.has_value())
                throw std::domain_error(fmt::format("ModularInt has no inverse: {}", a));
            a._value = (*a_opt)._value;
            n = -n;
        } else
            a._value = _value;

        mpz_powm_ui(a._value.value, a._value.value, n, _mod.value);
        return a;
    }

    ModularInt &ModularInt::operator+=(const ModularInt &other) {
        return op_set(addition<BigInt>, other);
    }

    ModularInt &ModularInt::operator-=(const ModularInt &other) {
        return op_set(subtraction<BigInt>, other);
    }

    ModularInt &ModularInt::operator*=(const ModularInt &other) {
        return op_set(multiplication<BigInt>, other);
    }

    ModularInt &ModularInt::operator/=(const ModularInt &other) {
        return op_set(division<BigInt>, other);
    }

    ModularInt &ModularInt::operator++() {
        _value = (++_value) % _mod;
        return *this;
    }

    ModularInt ModularInt::operator++(int) {
        ModularInt tmp{*this};
        _value = (_value + 1) % _mod;
        return tmp;
    }

    ModularInt &ModularInt::operator--() {
        _value = (--_value) % _mod;
        return *this;
    }

    ModularInt ModularInt::operator--(int) {
        ModularInt tmp{*this};
        _value = (_value - 1) % _mod;
        return tmp;
    }

    bool ModularInt::operator==(const ModularInt &other) const {
        check_same_mod(other);
        return _value == other._value;
    }

    bool ModularInt::operator<(const ModularInt &other) const {
        check_same_mod(other);
        return _value < other._value;
    }

    std::string ModularInt::to_string() const noexcept {
        return modular_int_string(_value, _mod);
    }

    ModularInt::Legendre ModularInt::legendre() const {
        // We use GMP functions here for efficiency.
        switch (mpz_legendre(_value.value, _mod.value)) {
            case  1: return Legendre::RESIDUE;
            case -1: return Legendre::NOT_RESIDUE;
            default: return Legendre::DIVIDES;
        }
    }

    bool ModularInt::residue() const {
        return legendre() == Legendre::RESIDUE;
    }

    std::optional<ModularInt> ModularInt::sqrt() const {
        // If a non-residue class, exit immediately.
        if (legendre() != Legendre::RESIDUE)
            return std::nullopt;

        // Test the last two bits of the modulus.
        // If they are both 1, then we can use Fermat's Little Theorem to calculate the result:
        // x = a^{(n+1)/4} (_mod n)
        // n+1 converts the last two bits from 1 to 0, and thus 4 divides n+1.
        // By Fermat:
        // a^n = a (_mod n)
        // a^{n+1} = a^2 (_mod n)
        // a^{(n+1)/4} = a^{1/2} (_mod n), which is exactly what we want.
        if (_mod.check_bit(0) && _mod.check_bit(1))
            return pow((_mod + 1) / 4);

        // Otherwise, we must use the Tonelli and Shanks method.
        // Initialize q to n - 1 (even), find # of zeros on right in binary representation, and eliminate them.
        // std::clog << "Invoking Tonelli and Shanks\n";
        auto q = _mod - 1;
        const auto e = mpz_scan1(q.value, 0);
        mpz_tdiv_q_2exp(q.value, q.value, e);

        // Find a generator.
        // Randomly search for non-residue.
        ModularInt n{1, _mod};
        while (n.legendre() != Legendre::NOT_RESIDUE)
            n = ModularInt{rng.random_mod(_mod.value), _mod};

        // Initialize working components.
        // y = n^q, where q is a BigInt, so the power is calculated with with the _mod of n.
        auto y = n.pow(q);
        auto r = e;

        // x = a^{{q-1}/2}
        // q-1 should be exactly divisible by 2 now.
        auto x = pow((q - 1) / 2);
        auto b = (*this) * x * x;
        x = (*this) * x;

        // Loop on algorithm until finished or failure. Terminate when b == 1.
        while (b._value != 1) {
            // Find minimum m such that b^{2m} = 1 (_mod p).
            auto m = 1;
            auto t1 = b;
            while (m < r) {
                t1 *= t1;
                if (t1._value == 1)
                    break;
                ++m;
            }

            // Should never happen as a is quadratic residue.
            if (r == m) {
#ifdef DEBUG
                std::clog << "ERROR: " << to_string() << " is not a quadratic residue!\n";
#endif
                throw std::domain_error(fmt::format("Unexpected error: {} is not a quadratic residue.", *this));
            }

            // Calculate t = y^{2^{r - m - 1}}.
            const auto shift_by = r - m - 1;
            const auto shift = 1 << shift_by;
            const auto t = y.pow(shift);

            y = t * t;
            r = m;
            x = x * t;
            b *= y;
        }

        return x;
    }

    std::optional<ModularInt> ModularInt::invert() const {
        // We use GMP functions here for efficiency.
        mpz_t result;
        mpz_init(result);
        const auto success = mpz_invert(result, _value.value, _mod.value);

        if (success) {
            ModularInt m{BigInt{result}, _mod};
            mpz_clear(result);
            return m;
        }

        mpz_clear(result);
        return std::nullopt;
    }

    void ModularInt::check_same_mod(const ModularInt &other) const {
        if (_mod != other._mod) {
            throw std::domain_error(fmt::format("Computation attempted with incompatible ModularInts: {} and {}.",
                                                *this, other));
        }
    }

    ModularInt ModularInt::op(const bigint_func1 &f) const {
        return ModularInt{f(_value), _mod};
    }

    ModularInt ModularInt::op(const bigint_func2 &f, const ModularInt &other) const {
        check_same_mod(other);
        return ModularInt{f(_value, other._value), _mod};
    }

    ModularInt &ModularInt::op_set(const bigint_func2 &f, const ModularInt &other) {
        check_same_mod(other);
        _value = f(_value, other._value) % _mod;
        return *this;
    }
}
