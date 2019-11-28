#include "Unsigned.hpp"

#include <algorithm>

namespace bignum
{

Unsigned::Unsigned()
: Unsigned(DigitSet())
{

}

Unsigned::Unsigned(const DigitSet& digits)
: digits_(digits)
{
}

Comparison compare(const Unsigned& lhs, const Unsigned& rhs)
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
        for(auto d = lhs.magnitude(); d > 0u; --d)
        {
            const auto digit = d - 1;
            if (lhs.digit(digit) < rhs.digit(digit))
            {
                return Comparison::LT;
            }
            else if (lhs.digit(digit) > rhs.digit(digit))
            {
                return Comparison::GT;
            }
        }
        return Comparison::EQ;
    }
}

bool operator==(const Unsigned& lhs, const Unsigned& rhs)
{
    switch (compare(lhs, rhs))
    {
    case Comparison::LT:
    case Comparison::GT: return false;
    case Comparison::EQ: return true;
    }
    return false;
}

bool operator!=(const Unsigned& lhs, const Unsigned& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const Unsigned& lhs, const Unsigned& rhs)
{
    switch(compare(lhs, rhs))
    {
    case Comparison::LT:
    case Comparison::EQ: return false;
    case Comparison::GT: return true;
    }
    return false;
}

bool operator<(const Unsigned& lhs, const Unsigned& rhs)
{
    return rhs > lhs;
}

bool operator>=(const Unsigned& lhs, const Unsigned& rhs)
{
    return !(lhs < rhs);
}

bool operator<=(const Unsigned& lhs, const Unsigned& rhs)
{
    return !(lhs > rhs);
}

}
