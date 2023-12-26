#pragma once

#include <concepts>
#include <span>

#include <bignum/Arrays/Comparators.hpp>

namespace bignum {

template<std::unsigned_integral U>
requires (!std::is_const_v<U>)
auto rightShift(
    std::span<U> result,
    typename std::span<U>::size_type shift
) -> std::span<U>::size_type {
    constexpr auto digitBitSize = sizeof(U) * 8;

    const auto wholeDigitShift = shift / digitBitSize;
    const auto bitShift = shift % digitBitSize;

    if (wholeDigitShift >= size(result)) {
        result[0] = 0;
        return 1;
    }

    if (bitShift) {
        result[0] = result[wholeDigitShift] >> bitShift;
        for (auto d = wholeDigitShift + 1; d < size(result); ++d) {
            result[d - wholeDigitShift - 1] |= result[d] << (digitBitSize - bitShift);
            result[d - wholeDigitShift    ]  = result[d] >>                 bitShift;
        }
    } else {
        for (auto d = wholeDigitShift; d < size(result); ++d) {
            result[d - wholeDigitShift] = result[d];
        }
    }

    return wholeDigitShift;
}

template<typename U, std::unsigned_integral K>
requires std::unsigned_integral<std::remove_const_t<U>>
auto leftShift(
    std::span<U> source,
    std::size_t bitShift,
    std::span<K> result
) -> void {
    constexpr auto digitBitSize = sizeof(U) * 8;

    if (!source) {
        return;
    }

    bitShift %= digitBitSize;

    if (!bitShift) {
        for (auto d = size(source); d > 0u; --d) {
            result[d - 1] = source[d - 1];
        }

        return;
    }

    for (auto d = size(source); d > 0u; --d) {
        const auto carry = source[d - 1] >> (digitBitSize - bitShift);
        if (carry) {
            result[d] |= carry;
        }

        result[d - 1] = source[d - 1] << bitShift;
    }
}

}