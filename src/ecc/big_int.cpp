/**
 * big_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include <iostream>
#include <format>
#include <stdexcept>
#include <string>
#include <gmp.h>

#include "gmp_ops.h"
#include "big_int.h"

namespace ecc {
    static const std::string div_error{"Division by zero."};
    static const std::string mod_error{"Modulus by zero."};

    BigInt::BigInt() {
        mpz_init(value);
        std::cerr << "BigInt default\n";
    }

    BigInt::BigInt(long value) {
        mpz_init_set_si(this->value, value);
        std::cerr << "BigInt long: " << value << '\n';
    }

    BigInt::BigInt(const std::string& str) {
        mpz_init_set_str(value, str.c_str(), 10);
        std::cerr << "BigInt string: " << str << '\n';
    }

    BigInt::BigInt(const mpz_t& value) {
        mpz_init_set(this->value, value);
        std::cerr << "BigInt mpz_t&: " << mpz_get_str(nullptr, 10, value) << '\n';
    }

    BigInt::BigInt(const BigInt &other) {
        mpz_init_set(value, other.value);
        std::cerr << "BigInt copy: " << mpz_get_str(nullptr, 10, value) << '\n';
    }

    BigInt::BigInt(BigInt &&other) noexcept {
        gmp_ops::mpz_move(value, other.value);
        gmp_ops::mpz_null(other.value);
        std::cerr << "BigInt &&: " << mpz_get_str(nullptr, 10, value) << '\n';
    }

    BigInt::~BigInt() {
        mpz_clear(value);
    }

    BigInt &BigInt::operator=(const BigInt &other) {
        std::cout << "BigInt =: " << mpz_get_str(nullptr, 10, value)
                  << ", other: " << mpz_get_str(nullptr, 10, other.value) << '\n';
        if (*this != other)
            mpz_set(this->value, other.value);
        return *this;
    }

    BigInt &BigInt::operator=(BigInt &&other) noexcept {
        std::cout << "BigInt &&=: " << mpz_get_str(nullptr, 10, value)
                  << ", other: " << mpz_get_str(nullptr, 10, other.value) << '\n';
        gmp_ops::mpz_move(value, other.value);
        gmp_ops::mpz_null(other.value);
        return *this;
    }

    BigInt BigInt::operator-() const {
        return op(mpz_neg);
    }

    BigInt BigInt::operator+(const BigInt &other) const {
        return op(mpz_add, other);
    }

    BigInt BigInt::operator-(const BigInt &other) const {
        return op(mpz_sub, other);
    }

    BigInt BigInt::operator*(const BigInt &other) const {
        return op(mpz_mul, other);
    }

    BigInt BigInt::operator/(const BigInt &other) const {
        other.check(div_error);
        return op(mpz_div, other);
    }

    BigInt BigInt::operator%(const BigInt &other) const {
        other.check(mod_error);
        return op(mpz_mod, other);
    }

    BigInt &BigInt::operator+=(const BigInt &other) {
        return op_set(mpz_add, other);
    }

    BigInt &BigInt::operator-=(const BigInt &other) {
        return op_set(mpz_sub, other);
    }

    BigInt &BigInt::operator*=(const BigInt &other) {
        return op_set(mpz_mul, other);
    }

    BigInt &BigInt::operator/=(const BigInt &other) {
        other.check(div_error);
        return op_set(mpz_div, other);
    }

    BigInt &BigInt::operator%=(const BigInt &other) {
        other.check(mod_error);
        return op_set(mpz_mod, other);
    }

    BigInt &BigInt::operator++() {
        mpz_add_ui(value, value, 1);
        return *this;
    }

    BigInt BigInt::operator++(int) {
        BigInt tmp(*this);
        mpz_add_ui(value, value, 1);
        return tmp;
    }

    BigInt &BigInt::operator--() {
        mpz_sub_ui(value, value, 1);
        return *this;
    }

    BigInt BigInt::operator--(int) {
        BigInt tmp(*this);
        mpz_sub_ui(value, value, 1);
        return tmp;
    }

    bool BigInt::operator==(const BigInt &other) const noexcept {
        return mpz_cmp(value, other.value) == 0;
    }

    bool BigInt::operator<(const BigInt &other) const noexcept {
        return mpz_cmp(value, other.value) < 0;
    }

    bool BigInt::zero() const noexcept {
        return mpz_cmp_si(this->value, 0) == 0;
    }

    BigInt BigInt::gcd(const BigInt &other) const noexcept {
        mpz_t a, b, g;

        mpz_inits(a, b, g, nullptr);
        mpz_set(a, this->value);
        mpz_set(b, other.value);

        mpz_gcd(g, a, b);

        mpz_clears(b, a, nullptr);
        return BigInt{g};
    }

    BigInt BigInt::extended_gcd(const BigInt &other, BigInt &x, BigInt &y) const noexcept {
        mpz_t a, b, g;

        mpz_inits(a, b, g, nullptr);
        mpz_set(a, this->value);
        mpz_set(b, other.value);

        mpz_gcdext(g, x.value, y.value, a, b);

        mpz_clears(b, a, nullptr);
        return BigInt{g};
    }

    int BigInt::check_bit(int pos) const noexcept {
        return mpz_tstbit(value, pos);
    }

    bool BigInt::is_probably_prime(int tries) const {
        if (tries <= 0)
            throw std::domain_error(std::format("Prime tries must be a positive integer: {}", tries));
        return mpz_probab_prime_p(value, tries);
    }

    std::string BigInt::to_string() const noexcept {
        const auto char_array = mpz_get_str(nullptr, 10, value);
        std::string str{char_array};
        free(char_array);
        return str;
    }

    BigInt::operator const mpz_t&() const {
        return value;
    }

    void BigInt::check(const std::string &err_msg) const {
        if (zero())
            throw std::domain_error(err_msg);
    }

    BigInt BigInt::op(const gmp_func1 &f) const {
        BigInt result;
        f(result.value, this->value);
        return result;
    }

    BigInt BigInt::op(const gmp_func2 &f, const BigInt &other) const {
        BigInt result;
        f(result.value, this->value, other.value);
        return result;
    }

    BigInt &BigInt::op_set(const gmp_func2 &f, const BigInt &other) {
        f(value, value, other.value);
        return *this;
    }
}
