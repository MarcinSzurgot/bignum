#pragma once

#include "../Integer.hpp"

namespace bignum
{

namespace details_
{

template<bool RhsSignInverted, typename DigitType>
Integer<DigitType> foo(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    using cmp = std::conditional_t<RhsSignInverted, std::equal_to<>, std::not_equal_to<>>;

    if (cmp{}(lhs.sign(), rhs.sign()))
    {
        const auto sign = lhs.abs() < rhs.abs() ? rhs.sign() : lhs.sign();
        return Integer(sign, lhs.abs() - rhs.abs());
    }
    else
    {
        return Integer(lhs.sign(), lhs.abs() + rhs.abs());
    }
}

}

template<typename DigitType>
Integer<DigitType> operator+(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return details_::foo<false>(lhs, rhs);
}

template<typename DigitType>
Integer<DigitType> operator-(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return details_::foo<true>(lhs, rhs);
}

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
