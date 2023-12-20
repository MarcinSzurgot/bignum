#pragma once

#include <concepts>
#include <cstdint>
#include <span>
#include <iostream>
#include <vector>

#include <bignum/ArrayArithmetics/ArrayShift.hpp>
#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/ArrayLogic/ArrayLogic.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<typename U>
requires std::unsigned_integral<std::remove_const_t<U>>
std::ostream& operator<<(std::ostream& os, std::span<U> num) {
    if (not empty(num)) {
        os << std::hex << "{0x" << num[size(num) - 1];
        for (auto d = size(num) - 1; d > 0u; --d) {
            os << ", 0x" << num[d - 1];
        }
    } else {
        os << "{";
    }
    return os << "}";
}

template<typename U>
requires std::unsigned_integral<std::remove_const_t<U>>
std::ostream& operator<<(std::ostream& os, const std::vector<U>& num) {
    return os << std::span(num);
}

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
auto divide(
    std::span<U1> lhs,
    std::span<U2> rhs,
    std::span<U3> quotient,
    std::span<U4> remainder
) -> std::pair<
    typename std::span<U3>::size_type, 
    typename std::span<U4>::size_type
> {
    constexpr auto digitBitSize = sizeof(U1) * 8;

    if (isZero(rhs)) {
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
    bignum::leftShift(rhs, bitDiff, std::span(divider).subspan(bitDiff / digitBitSize));

    while (std::span(remainder) >= rhs) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;

        divider.resize(size(divider) - bignum::rightShift(
            std::span(divider), 
            bitDiff - newBitDiff
        ));

        divider.resize(sizeWithoutLeadingZeroes(std::span(divider)));

        bitDiff = newBitDiff;

        if (std::span(divider) > std::span(remainder)) {
            bignum::rightShift(
                std::span(divider), 
                1
            );
            bitDiff--;
        }

        quotient[bitDiff / digitBitSize] |= U3(1) << (bitDiff % digitBitSize);

        bignum::subtract(
            remainder,
            std::span(divider)
        );

        remainder = remainder.subspan(0, sizeWithoutLeadingZeroes(remainder));
    }

    return {sizeWithoutLeadingZeroes(quotient), size(remainder)};
}

}