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
    class ModularInt;

    class BigInt {
        friend ModularInt;
    public:
        BigInt();
        BigInt(long);
        BigInt(const std::string&);
        BigInt(const mpz_t&);
        BigInt(const BigInt&);
        BigInt(BigInt&&) noexcept;

        ~BigInt();

        BigInt &operator=(const BigInt&);
        BigInt &operator=(BigInt&&) noexcept;

        [[nodiscard]] BigInt operator-() const;
        [[nodiscard]] BigInt operator+(const BigInt&) const;
        [[nodiscard]] BigInt operator-(const BigInt&) const;
        [[nodiscard]] BigInt operator*(const BigInt&) const;
        [[nodiscard]] BigInt operator/(const BigInt&) const;
        [[nodiscard]] BigInt operator%(const BigInt&) const;

        BigInt &operator+=(const BigInt&);
        BigInt &operator-=(const BigInt&);
        BigInt &operator*=(const BigInt&);
        BigInt &operator/=(const BigInt&);
        BigInt &operator%=(const BigInt&);

        BigInt &operator++();
        BigInt operator++(int);
        
        BigInt& operator--();
        BigInt operator--(int);

        // These define a total ordering on BigInt.
        [[nodiscard]] bool operator==(const BigInt&) const noexcept;
        [[nodiscard]] bool operator<(const BigInt&) const noexcept;

        [[nodiscard]] bool zero() const noexcept;

        // Find the GCD of this and the other number.
        [[nodiscard]] BigInt gcd(const BigInt&) const noexcept;

        // Find the GCD of this and the other number, and the coefficients that produce it.
        [[nodiscard]] BigInt extended_gcd(const BigInt&, BigInt&, BigInt&) const noexcept;

        // Determine if this number is (probably) prime.
        // Probable primality is checked the number of specified tries.
        // If the number of tries is not a positive integer, std::bad_domain is thrown.
        // If the function returns false, the number is guaranteed to be composite.
        [[nodiscard]] bool is_probably_prime(int tries = 1) const;

        [[nodiscard]] std::string to_string() const noexcept;

        [[nodiscard]] explicit operator const mpz_t&() const;

    private:
        mpz_t value;

        using gmp_func1 = std::function<void(mpz_ptr, mpz_srcptr)>;
        using gmp_func2 = std::function<void(mpz_ptr, mpz_srcptr, mpz_srcptr)>;

        // Raises a domain_error if this is zero for div and mod operations.
        void check(const std::string&) const;

        [[nodiscard]] BigInt op(const gmp_func1&) const;
        [[nodiscard]] BigInt op(const gmp_func2&, const BigInt&) const;
        [[nodiscard]] BigInt &op_set(const gmp_func2&, const BigInt&);
    };
}
