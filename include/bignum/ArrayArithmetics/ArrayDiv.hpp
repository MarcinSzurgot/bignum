#include <concepts>
#include <cstdint>
#include <span>

#include <bignum/ArrayLogic/ArrayLogic.hpp>
#include <bignum/Operations.hpp>

namespace bignum {

auto divide(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs,
    std::span<      std::uint32_t> quotient,
    std::span<      std::uint32_t> remainder
) -> void {

    if (isZero(rhs)) {
        throw std::runtime_error("Division by zero is not allowed!");
    }

    if (lhs < rhs) {
        std::fill(begin(quotient), end(quotient), 0);
        std::copy(begin(lhs), end(lhs), begin(remainder));
        return;
    }

    const auto rhsTopBit = topBit(rhs);
    const auto lhsTopBit = topBit(lhs);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = rhs << bitDiff;

    while (remainder >= rhs) {
        const auto newBitDiff = topBit(remainder) - rhsTopBit;

        divider >>= bitDiff - newBitDiff;
        bitDiff = newBitDiff;

        if (std::span(divider) > remainder) {
            divider >>= 1;
            bitDiff--;
        }

        quotient[bitDiff / digitBitSize] |= 1 << (bitDiff % digitBitSize);
        remainder -= divider.digits_;
        remainder.resize(sizeWithoutLeadingZeroes(remainder));
    }
}

}