#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <span>

namespace bignum {

template<typename Range1, typename Range2>
requires std::equality_comparable<std::ranges::range_value_t<Range1>> 
    && std::equality_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto equal(
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
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto less(
    Range1&& lhs,
    Range2&& rhs
) -> bool {
    // std::cout << "zaczynamy\n";
    if (size(lhs) != size(rhs)) {
        // std::cout << "porownanie: " << size(lhs) << ", " << size(rhs) << "\n";
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
auto greaterEqual(
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
auto greater(
    Range1&& lhs,
    Range2&& rhs
) -> bool { 
        // std::cout << "zaczynamy\n";
    return less(rhs, lhs); 
}

template<typename Range1, typename Range2>
requires std::three_way_comparable<std::ranges::range_value_t<Range1>> 
    && std::three_way_comparable<std::ranges::range_value_t<Range2>> 
    && std::same_as<
        std::ranges::range_value_t<Range1>, 
        std::ranges::range_value_t<Range2>
    >
auto lessEqual(
    Range1&& lhs,
    Range2&& rhs
) -> bool { return not greater(lhs, rhs); }

}