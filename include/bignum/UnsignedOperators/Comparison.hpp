#pragma once

#include "../Unsigned.hpp"

#include <iostream>

namespace bignum
{

template<typename DigitType> Comparison compare(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    if (lhs.magnitude() != rhs.magnitude())
    {
        return lhs.magnitude() < rhs.magnitude()
               ? Comparison::LT
               : Comparison::GT;
    }
    else
    {
        for (auto d = lhs.magnitude(); d > 0u; --d)
        {
            if (lhs[d - 1] < rhs[d - 1])
            {
                return Comparison::LT;
            }
            else if (lhs[d - 1] > rhs[d - 1])
            {
                return Comparison::GT;
            }
        }
        return Comparison::EQ;
    }
}

template<typename DigitType>
bool operator<(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::EQ:
    case Comparison::GT: return false;
    case Comparison::LT: return true;
    }
    throw std::invalid_argument("Invalid comparison value.");
}

template<typename DigitType>
bool operator>(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return rhs < lhs;
}

template<typename DigitType>
bool operator<=(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return !(lhs > rhs);
}

template<typename DigitType>
bool operator>=(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return !(lhs < rhs);
}

template<typename DigitType>
bool operator==(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::LT:
    case Comparison::GT: return false;
    case Comparison::EQ: return true;
    }
    throw std::invalid_argument("Invalid comparison value.");
}

template<typename DigitType>
bool operator!=(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return !(lhs == rhs);
}


}
