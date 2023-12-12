#pragma once

#include <concepts>
#include <cstdint>
#include <span>
#include <vector>

#include <bignum/ArrayArithmetics/ArrayShift.hpp>
#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/ArrayLogic/ArrayLogic.hpp>
#include <bignum/Operations.hpp>

namespace bignum {

template<std::unsigned_integral U>
inline std::ostream& operator<<(std::ostream& os, std::span<U> num) {
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

inline auto divide(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs,
    std::span<      std::uint32_t> quotient,
    std::span<      std::uint32_t> remainder
) -> std::pair<std::size_t, std::size_t> {
    constexpr auto digitBitSize = sizeof(std::uint32_t) * 8;

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
    auto divider = std::vector<std::uint32_t>(size(lhs));
    bignum::leftShift(rhs, bitDiff, std::span(divider).subspan(bitDiff / digitBitSize));

    while (std::span<const std::uint32_t>(remainder) >= rhs) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;

        divider.resize(size(divider) - bignum::rightShift(
            std::span(divider), 
            bitDiff - newBitDiff
        ));

        divider.resize(sizeWithoutLeadingZeroes(std::span(divider)));

        bitDiff = newBitDiff;

        if (std::span<const std::uint32_t>(divider) > std::span<const std::uint32_t>(remainder)) {
            bignum::rightShift(
                std::span(divider), 
                1
            );
            bitDiff--;
        }

        quotient[bitDiff / digitBitSize] |= 1 << (bitDiff % digitBitSize);

        bignum::subtract(
            remainder,
            std::span<const std::uint32_t>(divider)
        );

        remainder = remainder.subspan(0, sizeWithoutLeadingZeroes(remainder));
    }

    return {sizeWithoutLeadingZeroes(quotient), size(remainder)};
}

}