#pragma once

#include <bignum/Utils.hpp>

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>

namespace bignum {

template<std::ranges::bidirectional_range Range>
requires std::unsigned_integral<std::ranges::range_value_t<Range>>
constexpr auto zero(Range&& range) -> bool {
    return trimm(range) == begin(range);
}

template<
    std::ranges::bidirectional_range Range1,
    std::ranges::bidirectional_range Range2
> requires 
    std::unsigned_integral<std::ranges::range_value_t<Range1>>
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
    && std::ranges::sized_range<Range1>
    && std::ranges::sized_range<Range2>

constexpr auto equal(
    Range1&& lhs,
    Range2&& rhs
) -> bool {
    const auto nonZeroLeft  = std::ranges::subrange(std::begin(lhs), trimm(lhs));
    const auto nonZeroRight = std::ranges::subrange(std::begin(rhs), trimm(rhs));

    if (nonZeroLeft.size() != nonZeroRight.size()) {
        return false;
    }

    for (auto&& [left, right] : std::views::zip(
        nonZeroLeft, 
        nonZeroRight
    )) {
        if (left != right) {
            return false;
        }
    }

    return true;
}

template<
    std::ranges::bidirectional_range Range1,
    std::ranges::bidirectional_range Range2
> requires 
    std::unsigned_integral<std::ranges::range_value_t<Range1>>
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
    && std::ranges::sized_range<Range1>
    && std::ranges::sized_range<Range2>

constexpr auto less(
    Range1&& lhs,
    Range2&& rhs
) -> bool {
    const auto nonZeroLeft  = std::ranges::subrange(std::begin(lhs), trimm(lhs));
    const auto nonZeroRight = std::ranges::subrange(std::begin(rhs), trimm(rhs));

    if (nonZeroLeft.size() != nonZeroRight.size()) {
        return nonZeroLeft.size() < nonZeroRight.size();
    }

    for (auto&& [left, right] : std::views::zip(
        nonZeroLeft, 
        nonZeroRight
    ) | std::views::reverse) {
        if (left != right) {
            return left < right;
        }
    }

    return false;
}

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
constexpr auto greaterEqual(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return not less(lhs, rhs); }

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
constexpr auto greater(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return less(rhs, lhs); }

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
constexpr auto lessEqual(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return not greater(lhs, rhs); }

}