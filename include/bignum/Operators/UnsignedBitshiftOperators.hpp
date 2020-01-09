#pragma once

#include "../Unsigned.hpp"

namespace bignum
{

template<typename DigitType>
Unsigned<DigitType> operator<<
(
    const Unsigned<DigitType>& value,
    typename Unsigned<DigitType>::size_type offset
)
{
    constexpr auto bitsPerDigit = sizeof(DigitType) * 8u;

    const auto digitOffset = offset / bitsPerDigit;

    const auto newSize = value.magnitude() + digitOffset;

    auto digits = DigitSet(newSize, DigitType());
    
    return digits;
}

template<typename DigitType>
Unsigned<DigitType> operator>>
(
    const Unsigned<DigitType>& value,
    typename Unsigned<DigitType>::size_type offset
)
{

}

}
