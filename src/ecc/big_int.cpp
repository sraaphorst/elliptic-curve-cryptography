/**
 * big_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#ifdef DEBUG
#include <iostream>
#endif

#include <format>
#include <stdexcept>
#include <string>
#include <gmp.h>

#include "big_int.h"

namespace ecc {
    static const std::string div_error{"Division by zero."};
    static const std::string mod_error{"Modulus by zero."};

    BigInt::BigInt() {
        mpz_init(value);
#ifdef DEBUG
        std::clog << "BigInt default\n";
#endif
    }

    BigInt::BigInt(long l) {
        mpz_init_set_si(value, l);
#ifdef DEBUG
        std::clog << "BigInt long: " << l << '\n';
#endif
    }

    BigInt::BigInt(const std::string_view& input_view) {
        // We do need to ensure that the input_view is null-terminated, or we open ourselves to
        // security vulnerabilities or undefined behaviour.
        std::string str{input_view};
        mpz_init_set_str(value, str.data(), 10);
#ifdef DEBUG
        std::clog << "BigInt string: " << str << '\n';
#endif
    }

    BigInt::BigInt(const mpz_t& gmp) {
        mpz_init_set(value, gmp);
#ifdef DEBUG
        std::clog << "BigInt mpz_t&: " << mpz_get_str(nullptr, 10, gmp) << '\n';
#endif
    }

    BigInt::BigInt(const BigInt &other) {
        mpz_init_set(value, other.value);
#ifdef DEBUG
        std::clog << "BigInt copy: " << mpz_get_str(nullptr, 10, value) << '\n';
#endif
    }

    BigInt::BigInt(BigInt &&other) noexcept {
        mpz_swap(value, other.value);
        // This is needed and is the convention in mpz_class.
        mpz_init(other.value);
#ifdef DEBUG
        std::clog << "BigInt &&: " << mpz_get_str(nullptr, 10, value) << '\n';
#endif
    }

    BigInt::~BigInt() {
        mpz_clear(value);
    }

    BigInt &BigInt::operator=(const BigInt &other) {
#ifdef DEBUG
        std::clog << "BigInt =: " << mpz_get_str(nullptr, 10, value)
                  << ", other: " << mpz_get_str(nullptr, 10, other.value) << '\n';
#endif
        if (*this != other)
            mpz_set(value, other.value);
        return *this;
    }

    BigInt &BigInt::operator=(BigInt &&other) noexcept {
#ifdef DEBUG
            std::clog << "BigInt &&=: " << mpz_get_str(nullptr, 10, value)
                      << ", other: " << mpz_get_str(nullptr, 10, other.value) << '\n';
#endif
        std::swap(*value, *other.value);
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
        BigInt tmp{*this};
        mpz_add_ui(value, value, 1);
        return tmp;
    }

    BigInt &BigInt::operator--() {
        mpz_sub_ui(value, value, 1);
        return *this;
    }

    BigInt BigInt::operator--(int) {
        BigInt tmp{*this};
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
        return mpz_cmp_si(value, 0) == 0;
    }

    BigInt BigInt::gcd(const BigInt &other) const noexcept {
        mpz_t a, b, g;

        mpz_inits(a, b, g, nullptr);
        mpz_set(a, value);
        mpz_set(b, other.value);

        mpz_gcd(g, a, b);

        mpz_clears(b, a, nullptr);
        return BigInt{g};
    }

    BigInt BigInt::extended_gcd(const BigInt &other, BigInt &x, BigInt &y) const noexcept {
        mpz_t a, b, g;

        mpz_inits(a, b, g, nullptr);
        mpz_set(a, value);
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
        f(result.value, value);
        return result;
    }

    BigInt BigInt::op(const gmp_func2 &f, const BigInt &other) const {
        BigInt result;
        f(result.value, value, other.value);
        return result;
    }

    BigInt &BigInt::op_set(const gmp_func2 &f, const BigInt &other) {
        f(value, value, other.value);
        return *this;
    }
}
