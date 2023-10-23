/**
 * big_int.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <ostream>
#include <functional>
#include <string>

#include <gmp.h>

namespace ecc {
    class BigInt {
    public:
        BigInt();
        BigInt(long);
        BigInt(const std::string&);
        BigInt(const mpz_t&);
        BigInt(const BigInt&);
        BigInt(BigInt&&) noexcept;

        ~BigInt();

        [[nodiscard]] BigInt operator+(const BigInt&) const;
        [[nodiscard]] BigInt operator-() const;
        [[nodiscard]] BigInt operator-(const BigInt&) const;
        [[nodiscard]] BigInt operator*(const BigInt&) const;
        [[nodiscard]] BigInt operator/(const BigInt&) const;
        [[nodiscard]] BigInt operator%(const BigInt&) const;

        BigInt &operator=(const BigInt&);
        BigInt &operator=(BigInt&&) noexcept;

        BigInt &operator+=(const BigInt&);
        BigInt &operator-=(const BigInt&);
        BigInt &operator*=(const BigInt&);
        BigInt &operator/=(const BigInt&);
        BigInt &operator%=(const BigInt&);

        BigInt &operator++();
        BigInt operator++(int);
        
        BigInt& operator--();
        BigInt operator--(int);

        [[nodiscard]] bool operator==(const BigInt&) const noexcept;
        [[nodiscard]] bool operator!=(const BigInt&) const noexcept;
        [[nodiscard]] bool operator<(const BigInt&) const noexcept;
        [[nodiscard]] bool operator<=(const BigInt&) const noexcept;
        [[nodiscard]] bool operator>(const BigInt&) const noexcept;
        [[nodiscard]] bool operator>=(const BigInt&) const noexcept;

        [[nodiscard]] explicit operator mpz_t&();
        [[nodiscard]] explicit operator std::string() const;

    private:
        mpz_t value;

        using gmp_func1 = std::function<void(mpz_ptr, mpz_srcptr)>;
        using gmp_func2 = std::function<void(mpz_ptr, mpz_srcptr, mpz_srcptr)>;

        [[nodiscard]] BigInt op(const gmp_func1&) const;
        [[nodiscard]] BigInt op(const gmp_func2&, const BigInt&) const;
    };
}

std::ostream &operator<<(std::ostream&, const ecc::BigInt&);
