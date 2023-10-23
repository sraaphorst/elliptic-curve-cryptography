/**
 * big_int.cpp
 * By Sebastian Raaphorst, 2023.
 */

#include "big_int.h"

namespace ecc {
    BigInt::BigInt() {
        mpz_init(value);
    }

    BigInt::BigInt(long value) {
        mpz_init_set_si(this->value, value);
    }

    BigInt::BigInt(const std::string& str) {
        mpz_init_set_str(value, str.c_str(), 10);
    }

    BigInt::BigInt(const mpz_t& value) {
        mpz_init_set(this->value, value);
    }

    BigInt::BigInt(const BigInt &other) {
        mpz_init_set(value, other.value);
    }

    BigInt::BigInt(BigInt &&other) noexcept {
        mpz_init_set(value, other.value);
        mpz_init(other.value);
    }

    BigInt::~BigInt() {
        mpz_clear(value);
    }

    BigInt BigInt::operator+(const BigInt &other) const {
        return op(mpz_add, other);
    }

    BigInt BigInt::operator-() const {
        BigInt result;
        mpz_neg(result.value, value);
        return result;
    }

    BigInt BigInt::operator-(const BigInt &other) const {
        BigInt result;
        mpz_sub(result.value, value, other.value);
        return result;
    }

    BigInt BigInt::operator*(const BigInt &other) const {
        BigInt result;
        mpz_mul(result.value, value, other.value);
        return result;
    }

    BigInt BigInt::operator/(const BigInt &other) const {
        BigInt result;
        mpz_div(result.value, value, other.value);
        return result;
    }

    BigInt BigInt::operator%(const BigInt &other) const {
        BigInt result;
        mpz_mod(result.value, value, other.value);
        return result;
    }

    BigInt &BigInt::operator=(const BigInt &other) {
        if (this != &other)
            mpz_set(this->value, other.value);
        return *this;
    }

    BigInt &BigInt::operator=(BigInt &&other) noexcept {
        if (this != &other)
            mpz_swap(value, other.value);
        return *this;
    }

    BigInt &BigInt::operator+=(const BigInt &other) {
        mpz_add(value, value, other.value);
        return *this;
    }

    BigInt &BigInt::operator-=(const BigInt &other) {
        mpz_sub(value, value, other.value);
        return *this;
    }

    BigInt &BigInt::operator*=(const BigInt &other) {
        mpz_mul(value, value, other.value);
        return *this;
    }

    BigInt &BigInt::operator/=(const BigInt &other) {
        mpz_div(value, value, other.value);
        return *this;
    }

    BigInt &BigInt::operator%=(const BigInt &other) {
        mpz_mod(value, value, other.value);
        return *this;
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

    bool BigInt::operator!=(const BigInt &other) const noexcept {
        return !(*this == other);
    }

    bool BigInt::operator<(const BigInt &other) const noexcept {
        return mpz_cmp(value, other.value) < 0;
    }

    bool BigInt::operator<=(const BigInt &other) const noexcept {
        return mpz_cmp(value, other.value) <= 0;
    }

    bool BigInt::operator>(const BigInt &other) const noexcept {
        return mpz_cmp(value, other.value) > 0;
    }

    bool BigInt::operator>=(const BigInt &other) const noexcept {
        return mpz_cmp(value, other.value) >= 0;
    }

    BigInt::operator mpz_t& () {
        return value;
    }

    BigInt::operator std::string() const {
        const auto char_array = mpz_get_str(nullptr, 10, value);
        std::string str{char_array};
        free(char_array);
        return str;
    }

    BigInt BigInt::op(const gmp_func &f, const BigInt &other) const {
        BigInt result;
        f(result.value, this->value, other.value);
        return result;
    }
}

std::ostream &operator<<(std::ostream &out, const ecc::BigInt &big_int) {
    const std::string s = static_cast<std::string>(big_int);
    return out << s;
}