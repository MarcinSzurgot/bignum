#pragma once

#include <bignum/Unsigned.hpp>

#include <algorithm>
#include <cstdint>
#include <optional>
#include <type_traits>

#include <climits>

namespace bignum
{

template
<
    typename Integer,
    typename = std::enable_if_t<std::is_integral_v<Integer>>
>
std::optional<std::size_t> highestBitNumber(Integer value)
{
    if (!value)
    {
        return std::nullopt;
    }
    else
    {
        auto highest = sizeof(Integer) * CHAR_BIT - 1;
        for (; !(value & (Integer(1) << highest)); --highest);
        return highest;
    }
}

template<typename DigitType>
std::optional<std::size_t> highestBitNumber(const Unsigned<DigitType>& value)
{
    auto highest = highestBitNumber(value.msd());
    if (highest)
    {
        highest = highest.value() + sizeof(DigitType) * CHAR_BIT * (value.magnitude() - 1);
    }
    return highest;
}

}
