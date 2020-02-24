#pragma once

#include "../Integer.hpp"

namespace bignum
{

template<typename DigitType>
std::ostream& operator<<(std::ostream& os, const Integer<DigitType>& value)
{
    switch (value.sign())
    {
    case Sign::Minus: os << "-"; break;
    case Sign::Plus: break;
    }

    return os << value.abs();
}

template<typename DigitType>
std::istream& operator>>(std::istream& is, Integer<DigitType>& value)
{
    is >> std::ws;

    auto sign = Sign::Plus;
    if (const auto c = char(is.get()); c == '-')
    {
        sign = Sign::Minus;
    }
    else if (c != '+')
    {
        is.unget();
        if (!std::isdigit(c))
        {
            return is;
        }
    }

    auto abs = Unsigned<DigitType>(1u);
    is >> abs;
    if (!abs)
    {
        sign = Sign::Plus;
    }
    value = Integer<DigitType>(sign, std::move(abs));
    return is;
}

}
