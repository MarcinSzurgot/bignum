#pragma once

#include <iostream>
#include <string_view>

namespace bignum
{

enum class Comparison
{
    LT,
    EQ,
    GT
};

enum class Sign
{
    Minus,
    Plus
};

inline constexpr std::string_view string(Comparison comparison)
{
    switch (comparison)
    {
    case Comparison::LT: return "Comparison::LT";
    case Comparison::EQ: return "Comparison::EQ";
    case Comparison::GT: return "Comparison::GT";
    }
    throw std::invalid_argument("Invalid comparison value.");
}

inline constexpr std::string_view string(Sign sign)
{
    switch (sign)
    {
    case Sign::Minus: return "Sign::Minus";
    case Sign::Plus:  return "Sign::Plus";
    }
    throw std::invalid_argument("Invalid sign value.");
}

inline constexpr Sign negate(Sign sign)
{
    switch (sign)
    {
        case Sign::Minus: return Sign::Plus;
        case Sign::Plus:  return Sign::Minus;
    }
    throw std::invalid_argument("Invalid sign value.");
}

inline constexpr Sign operator~(Sign sign)
{
    return negate(sign);
}

inline std::ostream& operator<<(std::ostream& os, Comparison comparison)
{
    return os << string(comparison);
}

inline std::ostream& operator<<(std::ostream& os, Sign sign)
{
    return os << string(sign);
}

}
