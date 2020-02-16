#pragma once

#include <iosfwd>
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

constexpr std::string_view string(Comparison comparison)
{
    switch (comparison)
    {
    case Comparison::LT: return "Comparison::LT";
    case Comparison::EQ: return "Comparison::EQ";
    case Comparison::GT: return "Comparison::GT";
    }
    throw std::invalid_argument("Invalid comparison value.");
}

constexpr std::string_view string(Sign sign)
{
    switch (sign)
    {
    case Sign::Minus: return "Sign::Minus";
    case Sign::Plus:  return "Sign::Plus";
    }
    throw std::invalid_argument("Invalid sign value.");
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
