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
        for(auto d = lhs.magnitude() - 1u; d >= 0u; --d)
        {
            if (lhs.digit(d) < rhs.digit(d))
            {
                return Comparison::LT;
            }
            else if (lhs.digit(d) > rhs.digit(d))
            {
                return Comparison::GT;
            }
        }
        return Comparison::EQ;
    }
}

}
