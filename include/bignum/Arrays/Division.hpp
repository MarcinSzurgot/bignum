#pragma once

#include <bitset>
#include <concepts>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

#include <bignum/Arrays/Shifts.hpp>
#include <bignum/Arrays/AdditiveOperations.hpp>
#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<
    typename U1, 
    typename U2, 
    typename U3, 
    typename U4
> requires 
    std::unsigned_integral<std::remove_const_t<U1>>
    && std::unsigned_integral<std::remove_const_t<U2>>
    && std::unsigned_integral<U3>
    && std::unsigned_integral<U4>
    && std::same_as<std::remove_const_t<U1>, std::remove_const_t<U2>>
    && std::same_as<std::remove_const_t<U2>, U3>
    && std::same_as<U3, U4>
auto div(
    std::span<U1> lhs,
    std::span<U2> rhs,
    std::span<U3> quotient,
    std::span<U4> remainder
) -> std::pair<
    typename std::span<U3>::size_type, 
    typename std::span<U4>::size_type
> {
    if (!rhs) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    std::fill(begin(quotient), end(quotient), 0);
    std::copy(begin(lhs), end(lhs), begin(remainder));

    if (lhs < rhs) {
        return {size(quotient), size(remainder)};
    }

    const auto rhsTopBit = topBit(rhs);
    const auto lhsTopBit = topBit(lhs);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = std::vector<U3>(size(lhs));
    auto divSpan = std::span(divider);
    leftShift(rhs, bitDiff, divSpan.subspan(bitDiff / Bits<U1>::Size));

    while (remainder >= rhs) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;
        const auto newDivSize = rightShift(divSpan, bitDiff - newBitDiff);

        divSpan = divSpan.subspan(0, size(divSpan) - newDivSize);
        divSpan = divSpan.subspan(0, sizeWithoutLeadingZeroes(divSpan));

        bitDiff = newBitDiff;

        if (divSpan > remainder) {
            rightShift(divSpan, 1);
            bitDiff--;
        }

        quotient[bitDiff / Bits<U1>::Size] |= U3(1) << (bitDiff & Bits<U1>::ShiftMask);

        sub(remainder, divSpan);

        remainder = remainder.subspan(0, sizeWithoutLeadingZeroes(remainder));
    }

    return {
        size(quotient) - (quotient.back() == 0u), 
        size(remainder)
    };
}

}