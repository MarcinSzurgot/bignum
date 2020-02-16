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
std::istream operator>>(std::istream& is, Integer<DigitType>& value)
{
    return is;
}

}
