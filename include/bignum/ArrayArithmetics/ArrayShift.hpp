#pragma once

#include <concepts>
#include <span>

#include <bignum/ArrayLogic/ArrayLogic.hpp>

namespace bignum {

template<std::unsigned_integral U>
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

template<std::unsigned_integral U>
auto leftShift(
    std::span<const U> source,
    std::size_t shift,
    std::span<      U> result
) -> U {
    constexpr auto digitBitSize = sizeof(U) * 8;

    if (isZero(source)) {
        return U();
    }

    const auto wholeDigitsShift = shift / digitBitSize;
    const auto bitShift = shift % digitBitSize;

    std::copy(
        begin(source),
        end(source),
        begin(result) + wholeDigitsShift
    );

    if (!bitShift) {
        return U();
    }

    auto carry = U();

    for (auto& digit : result) {
        const auto newCarry = digit >> (digitBitSize - bitShift);
        digit <<= bitShift;
        digit |= carry;
        carry = newCarry;
    }

    return carry;
}

}