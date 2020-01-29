#pragma once

#include <bignum/Unsigned.hpp>
#include <bignum/Utility.hpp>

#include <random>
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

template<typename DigitType, typename Generator>
bignum::Unsigned<DigitType> randomUnsigned(Generator& generator, std::size_t maxDigitsCount)
{
    auto size = std::uniform_int_distribution{std::size_t(1), maxDigitsCount}(generator);
    auto digits = std::vector(size, DigitType());
    auto digitGenerator = std::uniform_int_distribution(DigitType(), std::numeric_limits<DigitType>::max());
    for (auto& digit : digits)
    {
        digit = digitGenerator(generator);
    }
    return bignum::Unsigned(std::move(digits));
}

template<typename DigitType>
bignum::Unsigned<DigitType> additionBasedMultiplication(const bignum::Unsigned<DigitType>& lhs,
                                                        const bignum::Unsigned<DigitType>& rhs)
{
    const auto lhsHighestBit = bignum::highestBitNumber(lhs);
    const auto rhsHighestBit = bignum::highestBitNumber(rhs);

    if (!lhsHighestBit || !rhsHighestBit)
    {
        return {};
    }

    const auto& less    = *lhsHighestBit < *rhsHighestBit ? lhs : rhs;
    const auto& greater = *lhsHighestBit < *rhsHighestBit ? rhs : lhs;
    const auto one      = bignum::Unsigned{DigitType(1)};

    auto result = bignum::Unsigned<DigitType>();
    const auto bits = std::min(*lhsHighestBit, *rhsHighestBit);
    for (auto bitNumber = 0u; bitNumber <= bits; ++bitNumber)
    {
        if (less & (one << bitNumber))
        {
            result += greater << bitNumber;
        }
    }
    return result;
}
