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

    auto digits = NonEmptyVector<DigitType>(newSize);

    return Unsigned(DigitSet(digits));
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
