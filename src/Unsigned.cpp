#include "Unsigned.hpp"

#include <algorithm>

namespace
{

void trimLeadingZeros(std::vector<bignum::digit_type>& digits)
{
    
}

}

namespace bignum
{

Unsigned::Unsigned()
: Unsigned({0})
{

}

Unsigned::Unsigned(std::vector<digit_type> digits)
: digits_(digits.empty() ? {0} : digits)
{

}

Comparison Unsigned::compare(const Unsigned& other) const
{
    if (magnitude() < other.magnitude())
    {
        return Comparison::LT;
    }
    else if (magnitude() > other.magnitude())
    {
        return Comparison::GT;
    }
    else
    {
        for(auto d = magnitude() - 1u; d >= 0u; --d)
        {
            if (digit(d) < other.digit(d))
            {
                return Comparison::LT;
            }
            else if (digit(d) > other.digit(d))
            {
                return Comparison::GT;
            }
        }

        return Comparison::EQ;
    }
}

}
