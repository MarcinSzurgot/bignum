#pragma once

#include "../Unsigned.hpp"

namespace bignum
{

template<typename DigitType>
Unsigned<DigitType> operator&(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less    = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result = less;
    for (auto digit = 0u; digit < less.magnitude(); ++digit)
    {
        result[digit] &= greater[digit];
    }
    result.trim();
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator|(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less    = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result = greater;
    for (auto digit = 0u; digit < less.magnitude(); ++digit)
    {
        result[digit] |= less[digit];
    }
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator^(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less    = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result = greater;
    for (auto digit = 0u; digit < less.magnitude(); ++digit)
    {
        result[digit] ^= less[digit];
    }
    result.trim();
    return result;
}


}
