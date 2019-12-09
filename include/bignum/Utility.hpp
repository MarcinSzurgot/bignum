#pragma once

#include "Defines.hpp"

#include <optional>
#include <type_traits>

namespace bignum
{

struct Unsigned;

template
<
    typename Integer,
    typename = typename std::enable_if<std::is_integral_v<Integer>>::type
>
std::optional<std::size_t> highestBitNumber(Integer value)
{
    for(auto bit = sizeof(Integer) * 8u; bit > 0u; --bit)
    {
        if (value & (Integer(1) << (bit - 1)))
        {
            return bit - 1;
        }
    }
    return std::nullopt;
}

std::optional<std::size_t> highestBitNumber(const Unsigned& value);

}
