/**
 * point_formatter.h
 * By Sebastian Raaphorst, 2023.
 */

#pragma once


#include "fmt/core.h"
#include "fmt/format.h"

#include "point.h"

template <>
struct fmt::formatter<ecc::Point> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const ecc::Point& p, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "({},{})", p.x().get_value(), p.y());
    }
};