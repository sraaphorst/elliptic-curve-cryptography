/**
 * big_int_formatting.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include "fmt/core.h"
#include "fmt/format.h"

#include "big_int.h"

template<>
struct fmt::formatter<ecc::BigInt> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ecc::BigInt &value, FormatContext &ctx) {
        return fmt::format_to(ctx.out(), "{}", value.to_string());
    }
};