#pragma once

#include <bitset>
#include <concepts>
#include <cstdint>
#include <span>
#include <vector>

namespace bignum {

namespace {

template<typename I>
requires std::integral<std::remove_const_t<I>>
auto leadingZeroes(
    std::span<I> digits
) -> std::span<I>::size_type {
    return std::find_if(
        rbegin(digits),
        rend(digits),
        [](auto digit) { return digit > 0; }
    ) - rbegin(digits);
}

}

template<typename I>
requires std::integral<std::remove_const_t<I>>
auto sizeWithoutLeadingZeroes(
    std::span<I> digits
) -> std::span<I>::size_type {
    return std::max(
        size(digits) - leadingZeroes(digits), 
        typename std::span<I>::size_type(1)
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