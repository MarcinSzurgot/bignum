#pragma once

#include "../Integer.hpp"

namespace bignum
{

template<typename DigitType>
Integer<DigitType> operator+(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    if (lhs.sign() == rhs.sign())
    {
        return Integer(lhs.sign(), lhs.abs() + rhs.abs());
    }
    else
    {
        const auto sign = [&]() {
            switch (compare(lhs.abs(), rhs.abs()))
            {
                case Comparison::LT: return rhs.sign();
                case Comparison::EQ: return Sign::Plus;
                case Comparison::GT: return lhs.sign();
            }
            throw std::invalid_argument("This should never happen.");   
        }();
        return Integer(sign, lhs.abs() - rhs.abs());
    }
}

template<typename DigitType>
Integer<DigitType> operator-(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    if (lhs.sign() != rhs.sign())
    {
        return Integer(lhs.sign(), lhs.abs() + rhs.abs());
    }
    else
    {
        const auto sign = [&]() {
            switch (compare(lhs.abs(), rhs.abs()))
            {
                case Comparison::LT: return negate(rhs.sign());
                case Comparison::EQ: return Sign::Plus;
                case Comparison::GT: return lhs.sign();
            }
            throw std::invalid_argument("This should never happen.");   
        }();
        return Integer(sign, lhs.abs() - rhs.abs());
    }
}

}
