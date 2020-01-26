#pragma mark

#include "AddDiff.hpp"

#include "../Unsigned.hpp"

#include <iostream>
#include <tuple>

namespace bignum
{

template<typename Integer>
std::string toBinString(Integer value)
{
    constexpr auto bitSize = sizeof(Integer) * CHAR_BIT;

    auto binary = std::string(bitSize, '0');
    for (auto& bit : binary)
    {
        bit += static_cast<bool>(value & (Integer(1) << (bitSize - 1)));
        value <<= 1;
    }
    return binary;
}

// TODO: Consider optimization for specific unsigned types.
template<typename DigitType>
constexpr std::pair<DigitType, DigitType> mul(DigitType lhs, DigitType rhs)
{
    static_assert(std::is_unsigned_v<DigitType>);

    constexpr auto bitSize = sizeof(DigitType) * CHAR_BIT;
    constexpr auto halfBitSize = bitSize / 2;
    constexpr auto mask = DigitType(~DigitType());
    constexpr auto lowerHalfMask = DigitType(mask >> halfBitSize);

    const auto lhsLowerHalf = DigitType(lhs & lowerHalfMask);
    const auto rhsLowerHalf = DigitType(rhs & lowerHalfMask);
    const auto lhsHigherHalf = DigitType(lhs >> halfBitSize);
    const auto rhsHigherHalf = DigitType(rhs >> halfBitSize);

    const auto lowerHalves = DigitType(lhsLowerHalf * rhsLowerHalf);
    const auto lowerHigher = DigitType(lhsLowerHalf * rhsHigherHalf);
    const auto higherLower = DigitType(lhsHigherHalf * rhsLowerHalf);

    const auto [mixedHalves, mixedHalvesCarry] = add(lowerHigher, higherLower);
    const auto [resultLowerHalf, resultLowerHalfCarry] = add(lowerHalves, DigitType(mixedHalves << halfBitSize));
    const auto resultHigherHalf = DigitType
    (
        lhsHigherHalf * rhsHigherHalf
        + (mixedHalves >> halfBitSize)
        + (mixedHalvesCarry << halfBitSize)
        + resultLowerHalfCarry
    );

    return {resultLowerHalf, resultHigherHalf};
}

template<typename DigitType>
Unsigned<DigitType> operator*(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    auto result = Unsigned(lhs.magnitude() + rhs.magnitude() + 1, DigitType());
    for (auto d1 = 0u; d1 < lhs.magnitude(); ++d1)
    {
        auto carry = false;
        for (auto d2 = 0u; d2 < rhs.magnitude(); ++d2)
        {
            const auto [a, b] = mul(lhs[d1], rhs[d2]);
            std::tie(result[d1 + d2 + 0], carry) = add(result[d1 + d2 + 0], a, carry);
            std::tie(result[d1 + d2 + 1], carry) = add(result[d1 + d2 + 1], b, carry);
        }

        for (auto d2 = rhs.magnitude(); carry; ++d2)
        {
            result[d1 + d2] += carry;
            carry = !result[d1 + d2];
        }
    }
    result.trim();
    return result;
}

template<typename DigitType> Unsigned<DigitType>
operator/(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return {};
}

}
