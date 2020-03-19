#pragma once

#include "../Unsigned.hpp"

#include <algorithm>

namespace bignum
{

template<typename DigitType_>
Unsigned<DigitType_> operator~(const Unsigned<DigitType_>& value)
{
    auto result = value;
    std::transform
    (
        begin(value.digits_),
        end(value.digits_),
        begin(result.digits_),
        std::bit_not<>{}
    );
    result.trim();
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator&(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less    = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result      = less;
    auto srcFirst1   = begin(greater.digits_);
    auto srcLast1    = std::next(srcFirst1, std::ptrdiff_t(less.magnitude()));
    auto srcFirst2   = begin(result.digits_);
    auto dstFirst    = begin(result.digits_);
    auto andOperator = std::bit_and<>{};

    std::transform(srcFirst1, srcLast1, srcFirst2, dstFirst, andOperator);

    result.trim();
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator|(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less    = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result = greater;
    std::transform
    (
        begin(less.digits_),
        end(less.digits_),
        begin(result.digits_),
        begin(result.digits_),
        std::bit_or<>{}
    );
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator^(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less    = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result = greater;
    std::transform
    (
        begin(less.digits_),
        end(less.digits_),
        begin(result.digits_),
        begin(result.digits_),
        std::bit_xor<>{}
    );
    result.trim();
    return result;
}

template<typename DigitType_>
Unsigned<DigitType_>& operator&=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs)
{
    return lhs = lhs & rhs;
}

template<typename DigitType_>
Unsigned<DigitType_>& operator|=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs)
{
    return lhs = lhs | rhs;
}

template<typename DigitType_>
Unsigned<DigitType_>& operator^=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs)
{
    return lhs = lhs ^ rhs;
}

}
