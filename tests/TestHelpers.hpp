#pragma once

#include <bignum/Unsigned.hpp>

#include <string>
#include <sstream>
#include <type_traits>

template<typename T>
std::string toString(const bignum::Unsigned<T>& value)
{
    auto stream = std::stringstream();
    stream << "{";
    for (auto d = 0u; d < value.magnitude(); ++d)
    {
        stream << +value[d] << ", ";
    }
    stream << "}";
    return stream.str();
}

template<typename Integer>
std::string toBinString(Integer value)
{
    static_assert(std::is_integral_v<Integer>);

    constexpr auto bitSize = sizeof(Integer) * CHAR_BIT;

    auto binary = std::string(bitSize, ' ');
    for (auto& bit : binary)
    {
        bit = value & (Integer(1) << (bitSize - 1)) ? '1' : '0';
        value <<= 1;
    }
    return binary;
}
