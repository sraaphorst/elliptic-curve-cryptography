/**
 * operations.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include <concepts>
#include <functional>

namespace ecc::operations {
    template <typename T>
    concept CanNegate = requires(const T& t1) {
        -t1;
    };

    template <CanNegate T>
    auto negation(const T &t1) {
        return -t1;
    }

    template <typename T>
    concept CanAdd = requires(const T& t1, const T& t2) {
        t1 + t2;
    };

    template <CanAdd T>
    auto addition(const T &t1, const T &t2) {
        return t1 + t2;
    }

    template <typename T>
    concept CanSubtract = requires(const T& t1, const T& t2) {
        t1 - t2;
    };

    template <CanSubtract T>
    auto subtraction(const T &t1, const T &t2) {
        return t1 - t2;
    }

    template <typename T>
    concept CanMultiply = requires(const T& t1, const T& t2) {
        t1 * t2;
    };

    template <CanMultiply T>
    auto multiplication(const T &t1, const T &t2) {
        return t1 * t2;
    }

    template <typename T>
    concept CanDivide = requires(const T& t1, const T& t2) {
        t1 / t2;
    };

    template <CanDivide T>
    auto division(const T &t1, const T &t2) {
        return t1 / t2;
    }

    template <typename T>
    concept CanTakeModulus = requires(const T& t1, const T& t2) {
        t1 % t2;
    };

    template <CanTakeModulus T>
    auto modulus(const T &t1, const T &t2) {
        return t1 % t2;
    }

    // Define equality and inequality based on EQ.
    // If we have LT, then we can define a partial or total ordering.
    template <typename T>
    concept EQ = requires(const T& t1, const T& t2) {
        t1 == t2;
    };

    template <EQ T>
    bool operator!=(const T &t1, const T &t2) {
        return !(t1 == t2);
    }

    template <typename T>
    concept LT = requires(const T &t1, const T &t2) {
        t1 < t2;
    };

    template <typename T>
    requires EQ<T> && LT<T>
    bool operator<=(const T &t1, const T &t2) {
        return t1 == t2 || t1 < t2;
    }

    template <typename T>
    requires EQ<T> && LT<T>
    bool operator>(const T &t1, const T &t2) {
        return !(t1 <= t2);
    }

    template <typename T>
    requires EQ<T> && LT<T>
    bool operator>=(const T &t1, const T &t2) {
        return !(t1 < t2);
    }
}
