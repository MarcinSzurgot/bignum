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
            std::cout << "result: " << result[d - wholeDigitShift    ] << std::endl;
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
    std::size_t bitShift,
    std::span<      U> result
) -> void {
    constexpr auto digitBitSize = sizeof(U) * 8;

    if (isZero(source)) {
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