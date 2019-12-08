#include "Unsigned.hpp"

#include "Utility.hpp"

#include <algorithm>

namespace
{

template<typename Integer>
std::string binary(Integer integer)
{
    constexpr auto one = Integer(1);
    auto binary = std::string(sizeof(Integer) * 8u, '0');
    for (auto bit = binary.size(); bit > 0u; --bit)
    {
        binary[binary.size() - bit] += (bool) (integer & (one << bit - 1));
    }
    return binary;
}

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
    const auto digitOffset = offset / bitsPerDigitType;
    const auto bitOffset = offset % bitsPerDigitType;
    const auto reversedBitOffset = bitsPerDigitType - bitOffset;
    const auto size = value.magnitude() + digitOffset + 1u;

    auto digits = bignum::DigitSet(size, bignum::digit_type());
    for(auto i = 0u; i < value.magnitude(); ++i)
    {
        digits[i + digitOffset + 0] |= bignum::digit_type(value.digit(i) << bitOffset);
        digits[i + digitOffset + 1] |= bignum::digit_type(value.digit(i) >> reversedBitOffset);
    }
    digits.trim();
    return {digits};
}

Unsigned operator>>(const Unsigned& value, Unsigned::size_type offset)
{
    if (offset >= bignum::highestBitNumber(value).value_or(0u))
    {
        return {};
    }

    const auto digitOffset = offset / bitsPerDigitType;
    const auto bitOffset = offset % bitsPerDigitType;
    const auto reversedBitOffset = bitsPerDigitType - bitOffset;
    const auto highestBit = *bignum::highestBitNumber(value.msd());

    auto digits = bignum::DigitSet
    (
        digitOffset >= value.magnitude() ? 1u : value.magnitude() - digitOffset
    );

    digits[0] |= value.digit(digitOffset) >> bitOffset;
    for (auto digit = 1u; digit < digits.size(); ++digit)
    {
        digits[digit - 0] |= value.digit(digit + digitOffset) >> bitOffset;
        digits[digit - 1] |= value.digit(digit + digitOffset) << reversedBitOffset;
    }

    digits.trim();
    return bignum::Unsigned(digits);
}

Unsigned operator+(const Unsigned& value)
{
    return value;
}

Unsigned operator-(const Unsigned& value)
{
    return value;
}

std::ostream& operator<<(std::ostream& os, const Unsigned& value)
{
    for(auto digit = value.magnitude(); digit > 0u; --digit)
    {
        os << +value.digit(digit - 1) << ", ";
    }
    return os;
}

}
