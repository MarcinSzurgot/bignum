#pragma once

#include "../Integer.hpp"

namespace bignum
{

template<typename DigitType>
Integer<DigitType> operator*(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return Integer(lhs.sign() != rhs.sign() ? Sign::Minus : Sign::Plus, lhs.abs() * rhs.abs());
}

template<typename DigitType>
Integer<DigitType> operator/(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return Integer(lhs.sign() != rhs.sign() ? Sign::Minus : Sign::Plus, lhs.abs() / rhs.abs());
}

template<typename DigitType>
Integer<DigitType> operator%(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return Integer(lhs);
}

}
