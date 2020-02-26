#pragma once

#include "../Integer.hpp"

namespace bignum
{

template<typename DigitType>
Integer<DigitType> operator~(const Integer<DigitType>& value)
{
    const auto sign = value.sign() == Sign::Minus ? Sign::Plus : Sign::Minus;
    return Integer<DigitType>(sign, ~value.abs());
}

template<typename DigitType>
Integer<DigitType> operator&(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    const auto sign = lhs.sign() == rhs.sign() && lhs.sign() == Sign::Minus
                      ? Sign::Minus
                      : Sign::Plus;
    return Integer<DigitType>(sign, lhs.abs() & rhs.abs());
}

template<typename DigitType>
Integer<DigitType> operator|(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    const auto sign = lhs.sign() == Sign::Minus || rhs.sign() == Sign::Minus
                      ? Sign::Minus
                      : Sign::Plus;
    return Integer<DigitType>(sign, lhs.abs() | rhs.abs());
}

template<typename DigitType>
Integer<DigitType> operator^(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    const auto sign = lhs.sign() != rhs.sign() ? Sign::Minus : Sign::Plus;
    return Integer<DigitType>(sign, lhs.abs() ^ rhs.abs());
}

template<typename DigitType>
Integer<DigitType>& operator&=(Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return lhs = lhs & rhs;
}

template<typename DigitType>
Integer<DigitType>& operator|=(Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return lhs = lhs | rhs;
}

template<typename DigitType>
Integer<DigitType>& operator^=(Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return lhs = lhs ^ rhs;
}

}
