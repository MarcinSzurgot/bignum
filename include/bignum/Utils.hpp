#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <span>

namespace bignum {

namespace {

template<std::ranges::input_range InputRange>
requires std::integral<std::ranges::range_value_t<InputRange>>
auto leadingZeroes(
    InputRange&& digits
) -> std::ranges::range_difference_t<InputRange> {
    auto reversed = std::views::reverse(digits);

    return std::ranges::find_if(reversed, [](auto&& digit) { 
        return digit > 0; 
    }) - begin(reversed);
}

}

template<std::ranges::input_range InputRange>
requires std::integral<std::ranges::range_value_t<InputRange>>
auto sizeWithoutLeadingZeroes(
    InputRange&& digits
) -> std::ranges::range_difference_t<InputRange> {
    return std::max(
        ssize(digits) - leadingZeroes(digits), 
        typename std::ranges::range_difference_t<InputRange>(1)
    );
}

template<typename I>
requires std::integral<std::remove_const_t<I>>
auto topBit(
    std::span<I> digits
) -> std::span<I>::size_type {
    constexpr auto digitBitSize = sizeof(I) * 8;

    if (empty(digits) || (size(digits) == 1u && digits.front() == 0u)) { 
        return 0u;
    }

    const auto topDigit = digits.back();

    auto bit = digitBitSize - 1;
    for (; bit > 0u && !((I(1) << bit) & topDigit); --bit) {

    }

    return bit + (size(digits) - 1) * digitBitSize;
}

}