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
    os << std::hex << "{0x" << num[0];
    for (auto d = 1u; d < size(num); ++d) {
        os << ", 0x" << num[d];
    }
    return os << "}";
}

auto divide(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs,
    std::span<      std::uint32_t> quotient,
    std::span<      std::uint32_t> remainder
) -> void {
    constexpr auto digitBitSize = sizeof(std::uint32_t) * 8;

    if (isZero(rhs)) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    std::fill(begin(quotient), end(quotient), 0);
    std::copy(begin(lhs), end(lhs), begin(remainder));

    if (lhs < rhs) {
        return;
    }

    const auto rhsTopBit = topBit(rhs);
    const auto lhsTopBit = topBit(lhs);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = std::vector<std::uint32_t>(size(lhs));
    bignum::leftShift(rhs, bitDiff, std::span(divider).subspan(bitDiff / digitBitSize));
    // remainder = remainder.subspan(0, sizeWithoutLeadingZeroes(remainder));
    // auto divider = rhs << bitDiff;

    while (std::span<const std::uint32_t>(remainder) >= rhs) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;

        bignum::rightShift(
            std::span(divider), 
            bitDiff - newBitDiff
        );

        // divider >>= bitDiff - newBitDiff;
        bitDiff = newBitDiff;

        if (std::span<const std::uint32_t>(divider) > std::span<const std::uint32_t>(remainder)) {
            bignum::rightShift(
                std::span(divider), 
                bitDiff - newBitDiff
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
}

}