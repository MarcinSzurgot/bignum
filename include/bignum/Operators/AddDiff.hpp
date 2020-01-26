#pragma once

#include "../Unsigned.hpp"

#include <tuple>

namespace bignum
{

template<typename DigitType>
constexpr std::pair<DigitType, DigitType> add(DigitType lhs, DigitType rhs, bool carry = false)
{
    static_assert(std::is_unsigned_v<DigitType>);

    auto sum = DigitType(lhs + rhs);
    const auto overflow = sum < lhs || sum < rhs;
    sum += carry;
    carry = overflow || (carry && !sum);
    return {sum, carry};
}

template<typename DigitType>
constexpr std::pair<DigitType, DigitType> diff(DigitType lhs, DigitType rhs, bool carry = false)
{
    static_assert(std::is_unsigned_v<DigitType>);

    auto diff = DigitType(lhs - rhs);
    const auto underflow = diff > lhs;
    diff -= carry;
    carry = underflow || (carry && (diff == std::numeric_limits<DigitType>::max()));
    return {diff, carry};
}

template<typename DigitType>
Unsigned<DigitType> operator+(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto& less = lhs.magnitude() < rhs.magnitude() ? lhs : rhs;
    const auto& greater = lhs.magnitude() < rhs.magnitude() ? rhs : lhs;

    auto result = Unsigned(greater.magnitude() + 1, DigitType());
    auto carry = false;
    for (auto d = 0u; d < less.magnitude(); ++d)
    {
        std::tie(result[d], carry) = add(less[d], greater[d], carry);
    }

    for (auto d = less.magnitude(); d < greater.magnitude(); ++d)
    {
        std::tie(result[d], carry) = add(DigitType(), greater[d], carry);
    }

    if (carry)
    {
        result[result.magnitude() - 1] = carry;
    }
    else
    {
        result.digits_.pop_back();
    }
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator-(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto isLhsLess = lhs < rhs;
    const auto& less = isLhsLess ? lhs : rhs;
    const auto& greater = isLhsLess ? rhs : lhs;

    auto result = Unsigned(greater.magnitude(), DigitType());
    auto carry = false;
    for (auto d = 0u; d < less.magnitude(); ++d)
    {
        std::tie(result[d], carry) = diff(greater[d], less[d], carry);
    }

    for (auto d = less.magnitude(); d < greater.magnitude(); ++d)
    {
        std::tie(result[d], carry) = diff(greater[d], DigitType(), carry);
    }

    result.trim();
    return result;
}

template<typename DigitType>
Unsigned<DigitType>& operator+=(Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return lhs = lhs + rhs;
}

template<typename DigitType>
Unsigned<DigitType>& operator-=(Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    return lhs = lhs - rhs;
}

}
