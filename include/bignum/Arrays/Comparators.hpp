#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>

namespace bignum {

template<typename Range>
requires std::ranges::forward_range<Range>
    && std::ranges::sized_range<Range>
    && std::equality_comparable<std::ranges::range_value_t<Range>>
auto operator!(Range&& range) -> bool {
    return size(range) == typename std::ranges::range_size_t<Range>(1) 
    && *begin(range) == typename std::ranges::range_value_t<Range>();
}

template<typename Range1, typename Range2>
requires std::equality_comparable<std::ranges::range_value_t<Range1>> 
    && std::equality_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto operator==(
    Range1&& lhs,
    Range2&& rhs
) -> bool {
    if (size(lhs) != size(rhs)) {
        return false;
    }

    return std::ranges::all_of(
        std::views::zip(lhs, rhs), 
        [](auto&& x) { return x.first == x.second; }
    );
}

template<typename Range1, typename Range2>
requires std::equality_comparable<std::ranges::range_value_t<Range1>> 
    && std::equality_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto operator!=(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return !(lhs == rhs); }

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto operator<(
    Range1&& lhs,
    Range2&& rhs
) -> bool {
    if (size(lhs) != size(rhs)) {
        return size(lhs) < size(rhs);
    }

    for (auto&& [l, r] : std::views::zip(lhs, rhs) | std::views::reverse) {
        if (l != r) {
            return l < r;
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
auto operator>=(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return !(lhs < rhs); }

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto operator>(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return rhs < lhs; }

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto operator<=(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return !(lhs > rhs); }

}