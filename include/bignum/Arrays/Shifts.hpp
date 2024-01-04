#pragma once

#include <concepts>
#include <span>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Digits/Arithmetics.hpp>

namespace bignum {

template<std::unsigned_integral U>
requires (!std::is_const_v<U>)
auto rightShift(
    std::span<U> result,
    typename std::span<U>::size_type shift
) -> std::span<U>::size_type {
    const auto [wholeDigitShift, bitShift] = div<U>(shift, Bits<U>::Size);

    if (wholeDigitShift >= size(result)) {
        result[0] = 0;
        return 1;
    }

    result[0] = rshift(result[wholeDigitShift], bitShift).second;
    for (auto d = wholeDigitShift + 1; d < size(result); ++d) {
        const auto [lower, upper] = rshift(result[d], bitShift);
        result[d - wholeDigitShift - 1] |= lower;
        result[d - wholeDigitShift    ]  = upper;
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
    bitShift &= Bits<U>::ShiftMask;

    auto lower = U();
    auto upper = U();
    auto previousLower = U();

    for (auto d = size(source); d > 0u; --d) {
        std::tie(lower, upper) = lshift(source[d - 1], bitShift);

        // previousLower |= upper;
        
        result[d - 1]  = lower;
        result[d    ] |= upper;
    }
}

}