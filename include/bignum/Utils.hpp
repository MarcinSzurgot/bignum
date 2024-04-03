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

template<typename InputRange>
requires std::unsigned_integral<std::ranges::range_value_t<InputRange>>
    && std::ranges::sized_range<InputRange>
    && std::ranges::bidirectional_range<InputRange>
auto topBit(
    InputRange&& digits
) -> std::ranges::range_size_t<InputRange> {
    using Size     = std::ranges::range_size_t<InputRange>;
    using Unsigned = std::ranges::range_value_t<InputRange>;

    if (empty(digits) || (size(digits) == Size(1) && *begin(digits) == Unsigned())) { 
        return 0u;
    }

    const auto topDigit = *(end(digits) - 1);

    auto bit = Bits<Unsigned>::Size - 1;
    for (; bit > 0u && !((Unsigned(1) << bit) & topDigit); --bit) {

    }

    return bit + (size(digits) - 1) * Bits<Unsigned>::Size;
}

}