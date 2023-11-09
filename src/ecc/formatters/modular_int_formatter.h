/**
 * modular_int_formatter.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once

#include "fmt/core.h"
#include "fmt/format.h"

#include "modular_int.h"

template <>
struct fmt::formatter<ecc::ModularInt> {
    // You can store parsed arguments here if necessary

    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        // Implement parsing logic here if needed, e.g., handling different formats
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const ecc::ModularInt& mi, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "{}", mi.to_string());
    }
};