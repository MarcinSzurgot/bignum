#pragma once

#include "../Integer.hpp"

namespace bignum
{

template<typename DigitType> Comparison compare(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    if (lhs.sign() != rhs.sign())
    {
        switch (lhs.sign())
        {
        case Sign::Minus: return Comparison::LT;
        case Sign::Plus:  return Comparison::GT;
        }
    }
    else
    {
        switch (lhs.sign())
        {
        case Sign::Minus: return compare(rhs.abs(), lhs.abs());
        case Sign::Plus:  return compare(lhs.abs(), rhs.abs());
        }
    }
    throw std::invalid_argument("Invalid comparison value.");
}

template<typename DigitType> bool operator <(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::EQ:
    case Comparison::GT: return false;
    case Comparison::LT: return true;
    }
    throw std::invalid_argument("Invalid comparison value.");
}

template<typename DigitType> bool operator >(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return rhs < lhs;
}

template<typename DigitType> bool operator<=(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return !(lhs > rhs);
}

template<typename DigitType> bool operator>=(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return !(lhs < rhs);
}

template<typename DigitType> bool operator==(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::GT:
    case Comparison::LT: return false;
    case Comparison::EQ: return true;
    }
    throw std::invalid_argument("Invalid comparison value.");
}

template<typename DigitType> bool operator!=(const Integer<DigitType>& lhs, const Integer<DigitType>& rhs)
{
    return !(lhs == rhs);
}

}
