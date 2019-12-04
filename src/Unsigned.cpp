#include "Unsigned.hpp"

#include "Utility.hpp"

#include <algorithm>

namespace
{

constexpr auto bitsPerDigit = sizeof(bignum::digit_type) * 8u;

}

namespace bignum
{

Unsigned::Unsigned()
: Unsigned(DigitSet())
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

Unsigned operator<<(const Unsigned& value, Unsigned::size_type offset)
{
    const auto digitOffset = offset / bitsPerDigit;
    const auto bitOffset = offset % bitsPerDigit;
    const auto reversedBitOffset = bitsPerDigit - bitOffset;
    const auto size = value.magnitude() + digitOffset + 1u;

    auto digits = bignum::DigitSet(size, bignum::digit_type());
    for(auto i = 0u; i < value.magnitude(); ++i)
    {
        digits[i + digitOffset + 0] |= value.digit(i) << bitOffset;
        digits[i + digitOffset + 1] |= value.digit(i) >> reversedBitOffset;
    }
    digits.trim();
    return {digits};
}

Unsigned operator>>(const Unsigned& value, Unsigned::size_type offset)
{
    const auto digitOffset = offset / bitsPerDigit;
    const auto bitOffset = offset % bitsPerDigit;
    const auto reversedBitOffset = bitsPerDigit - bitOffset;
    const auto size = value.magnitude() - std::min(digitOffset, value.magnitude());

    auto digits = bignum::DigitSet(size, bignum::digit_type());

    if (digits.size() > 2u)
    {
        for(auto i = 1u; i < digits.size(); ++i)
        {
            digits[i - 0] |= value.digit(digitOffset) >> bitOffset;
            digits[i - 1] |= value.digit(digitOffset) << reversedBitOffset;
        }
    }
    else
    {
        digits[0] = value.digit(digitOffset) >> bitOffset;
    }
    digits.trim();
    return {digits};
}

Unsigned operator+(const Unsigned& value)
{
    return value;
}

Unsigned operator-(const Unsigned& value)
{
    return value;
}

}
