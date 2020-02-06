#pragma once

#include "../Unsigned.hpp"

#include <iostream>

namespace bignum
{

template<typename DigitType>
std::ostream& operator<<(std::ostream& os, const Unsigned<DigitType>& value)
{
    static const auto ten = Unsigned<DigitType>(10u);

    if (!value)
    {
        return os << '0';
    }
    else
    {
        auto string = std::string();
        for (auto copy = value; copy; copy /= ten)
        {
            const auto remainder = copy % ten;
            string.insert(begin(string), remainder.lsd() + '0');
        }
        return os << string;
    }
}

template<typename DigitType>
std::istream& operator>>(std::istream& is, Unsigned<DigitType>& value)
{
    static const auto ten = Unsigned<DigitType>(10u);

    auto tmp = Unsigned<DigitType>();
    value = {};
    for (auto c = is.get(); static_cast<bool>(is); c = is.get())
    {
        if (!std::isdigit(c))
        {
            is.unget();
            break;
        }

        if (value)
        {
            value *= ten;
        }
        tmp[0] = c - '0';
        value += tmp;
    }
    return is;
}

}
