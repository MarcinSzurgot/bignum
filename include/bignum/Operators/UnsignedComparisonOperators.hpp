#pragma once

#include "../Unsigned.hpp"

namespace bignum
{

template<typename DigitType>
Comparison compare(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    if (lhs.magnitude() < rhs.magnitude())
    {
        return Comparison::LT;
    }
    else if (lhs.magnitude() > rhs.magnitude())
    {
        return Comparison::GT;
    }
    else
    {
        for (auto counter = 1u; counter <= lhs.magnitude(); ++counter)
        {
            const auto digit = lhs.magnitude() - counter;
            if (lhs[digit] < rhs[digit])
            {
                return Comparison::LT;
            }
            else if (lhs[digit] > rhs[digit])
            {
                return Comparison::GT;
            }
        }

        return Comparison::EQ;
    }
}

template<typename DigitType>
bool operator==(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::GT:
    case Comparison::LT: return false;
    case Comparison::EQ: return true;
    }
}

template<typename DigitType>
bool operator!=(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return !(lhs == rhs);
}

template<typename DigitType>
bool operator>(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::LT:
    case Comparison::EQ: return false;
    case Comparison::GT: return true;
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
}

template<typename DigitType>
bool operator>=(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return !(lhs < rhs);
}

template<typename DigitType>
bool operator<=(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return !(rhs > lhs);
}

}
