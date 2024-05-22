#pragma once

#include <bignum/Bits.hpp>

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <span>

namespace bignum {

template<std::ranges::bidirectional_range Range>
requires std::convertible_to<std::ranges::range_value_t<Range>, bool>
auto trimm(Range&& range) -> std::ranges::iterator_t<Range> {
    return (
        range
        | std::views::reverse
        | std::views::drop_while(std::logical_not<>{})
        | std::views::reverse
    ).end().base().base();
}

}