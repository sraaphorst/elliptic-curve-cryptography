/**
 * ModularInt.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include "modular_int.h"

#include <format>
#include <stdexcept>
#include "operations.h"

namespace ecc {
    using namespace operations;

    // The representation of a ModularInt.
    static std::string modular_int_string(const BigInt &value, const BigInt &mod) {
        return std::format("{}({})", value.to_string(), mod.to_string());
    }

    // Make sure we reduce in case value >= mod.
    ModularInt::ModularInt(BigInt value, BigInt mod): value{std::move(value)}, mod{std::move(mod)} {
        if (this->mod.zero())
            throw std::domain_error(std::format("Error: tried to create {}.", modular_int_string(this->value, this->mod)));
        this->value %= this->mod;
    }

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
        value = (++value) % mod;
        return *this;
    }

    ModularInt ModularInt::operator++(int) {
        ModularInt tmp(*this);
        value = (++value) % mod;
        return tmp;
    }

    ModularInt &ModularInt::operator--() {
        value = (--value) % mod;
        return *this;
    }

    ModularInt ModularInt::operator--(int) {
        ModularInt tmp(*this);
        value = (--value) % mod;
        return tmp;
    }

    bool ModularInt::operator==(const ModularInt &other) const {
        check_same_mod(other);
        return value == other.value;
    }

    bool ModularInt::operator<(const ModularInt &other) const {
        check_same_mod(other);
        return value < other.value;
    }

    std::string ModularInt::to_string() const {
        return std::format("{}({})", value.to_string(), mod.to_string());
    }

    void ModularInt::check_same_mod(const ModularInt &other) const {
        if (this->mod != other.mod)
            throw std::domain_error(std::format("Computation with incompatible mod: {} and {}.",
                                    this->to_string(), other.to_string()));
    }

    ModularInt ModularInt::op(const bigint_func1 &f) const {
        return ModularInt(f(value), mod);
    }

    ModularInt ModularInt::op(const bigint_func2 &f, const ModularInt &other) const {
        check_same_mod(other);
        return ModularInt(f(this->value, other.value), mod);
    }

    ModularInt &ModularInt::op_set(const bigint_func2 &f, const ModularInt &other) {
        check_same_mod(other);
        this->value = f(this->value, other.value);
        return *this;
    }
}
