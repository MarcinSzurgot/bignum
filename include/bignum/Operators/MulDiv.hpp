#pragma once

#include "AddDiff.hpp"

#include "../Unsigned.hpp"

#include <limits>
#include <tuple>

namespace bignum
{

namespace _details
{

// divisor must be non-zero
template<typename DigitType>
constexpr std::pair<DigitType, DigitType> div(DigitType dividend, DigitType divisor)
{
    static_assert(std::is_unsigned_v<DigitType>);
    const auto quotient = dividend / divisor;
    const auto remainder = dividend - divisor * quotient;
    return {quotient, remainder};
}

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

// divisor must be non-zero
template<typename DigitType>
constexpr std::pair<DigitType, DigitType> div(std::pair<DigitType, DigitType> dividend, DigitType divisor)
{
    static_assert(std::is_unsigned_v<DigitType>);

    constexpr auto max = std::numeric_limits<DigitType>::max();

    const auto [higherQuotient, higherRemainder] = _details::div(dividend.second, divisor);
    const auto [maximumQuotient, maximumRemqinder] = _details::div(max, divisor);
    const auto lowerQuotient = DigitType
    (
        higherRemainder
        * maximumQuotient
        + (higherRemainder
           * (maximumRemqinder + 1)
           + dividend.first)
          / divisor
    );
    return {lowerQuotient, higherQuotient};
}

template<typename DigitType>
Unsigned<DigitType> operator*(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    auto result = Unsigned(lhs.magnitude() + rhs.magnitude(), DigitType());
    for (auto d1 = 0u; d1 < lhs.magnitude(); ++d1)
    {
        auto carry = false;
        for (auto d2 = 0u; d2 < rhs.magnitude(); ++d2)
        {
            const auto [a, b] = mul(lhs[d1], rhs[d2]);
            std::tie(result[d1 + d2 + 0], carry) = add(result[d1 + d2 + 0], a, carry);
            std::tie(result[d1 + d2 + 1], carry) = add(result[d1 + d2 + 1], b, carry);

            for (auto c = 2u; carry; ++c)
            {
                std::tie(result[d1 + d2 + c], carry) = add(result[d1 + d2 + c], DigitType(carry));
            }
        }
    }
    result.trim();
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator/(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    constexpr auto bitSize = sizeof(DigitType) * CHAR_BIT;

    if (!rhs)
    {
        throw std::invalid_argument("Cannot divide by zero.");
    }
    else
    {
        auto result = Unsigned
        (
            lhs.magnitude() > rhs.magnitude()
            ? lhs.magnitude() - rhs.magnitude()
            : 1,
            DigitType()
        );

        auto multiplier = Unsigned<DigitType>();
        auto shift = decltype(rhs.magnitude())(0);
        for (auto dividend = lhs; dividend >= rhs;)
        {
            const auto dividendMsd = dividend.msd();
            const auto divisorMsd  = rhs.msd();
            if (dividendMsd <= divisorMsd)
            {
                if (dividend.magnitude() == rhs.magnitude())
                {
                    shift = 0;
                    multiplier = Unsigned{DigitType(1)};
                }
                else
                {
                    const auto dividendPrelastMsd = dividend[dividend.magnitude() - 2];
                    if (divisorMsd == std::numeric_limits<DigitType>::max())
                    {
                        multiplier = Unsigned{dividendMsd};
                    }
                    else
                    {
                        const auto [lowerDigit, higherDigit] = div({dividendPrelastMsd, dividendMsd}, DigitType(divisorMsd + 1));
                        multiplier = Unsigned{lowerDigit, higherDigit};
                    }
                    shift = dividend.magnitude() - (rhs.magnitude() + 1);
                }
            }
            else
            {
                shift = dividend.magnitude() - rhs.magnitude();
                multiplier = Unsigned<DigitType>(DigitType(dividendMsd / (divisorMsd + 1)));
                if (!multiplier)
                {
                    multiplier[0] = 1;
                }
            }
            result += multiplier << shift * bitSize;
            dividend -= (rhs * multiplier) << (shift * bitSize);
        }
        result.trim();
        return result;
    }
}

template<typename DigitType>
Unsigned<DigitType> operator%(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto quotient = lhs / rhs;
    const auto multiplied = quotient * rhs;
    return lhs - multiplied;
}

template<typename DigitType>
Unsigned<DigitType>& operator*=(Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return lhs = lhs * rhs;
}

template<typename DigitType>
Unsigned<DigitType>& operator/=(Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return lhs = lhs / rhs;
}

template<typename DigitType>
Unsigned<DigitType>& operator%=(Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return lhs = lhs % rhs;
}

}
