#pragma once

#include "../Unsigned.hpp"

#include <tuple>

namespace bignum
{

template<typename DigitType>
constexpr std::pair<DigitType, bool> add(DigitType lhs, DigitType rhs, bool carry = false)
{
    static_assert(std::is_unsigned_v<DigitType>);

    auto sum = DigitType(lhs + rhs);
    const auto overflow = sum < lhs || sum < rhs;
    sum += carry;
    carry = overflow || (carry && !sum);
    return {sum, carry};
}

template<typename DigitType>
constexpr std::pair<DigitType, bool> diff(DigitType lhs, DigitType rhs, bool carry = false)
{
    static_assert(std::is_unsigned_v<DigitType>);

    auto diff = DigitType(lhs - rhs);
    const auto underflow = diff > lhs;
    diff -= carry;
    carry = underflow || (carry && (diff == std::numeric_limits<DigitType>::max()));
    return {diff, carry};
}

namespace details_
{

template<typename ForwardOutputIterator, typename ForwardInputIterator>
void iteratorAdd(ForwardOutputIterator outputFirst, ForwardInputIterator lessFirst, ForwardInputIterator lessLast)
{
    auto carry = false;
    for (; lessFirst != lessLast; ++outputFirst, ++lessFirst)
    {
        std::tie(*outputFirst, carry) = add(*outputFirst, *lessFirst, carry);
    }

    for (; carry; ++outputFirst)
    {
        *outputFirst += carry;
        carry = !*outputFirst;
    }
}

template<typename ForwardOutputIterator, typename ForwardInputIterator>
void iteratorDiff(ForwardOutputIterator outputFirst, ForwardInputIterator inputFirst, ForwardInputIterator inputLast)
{
    auto carry = false;
    for (; inputFirst != inputLast; ++outputFirst, ++inputFirst)
    {
        std::tie(*outputFirst, carry) = diff(*outputFirst, *inputFirst, carry);
    }

    using digit_type = typename decltype(outputFirst)::value_type;
    for (; carry; ++outputFirst)
    {
        *outputFirst -= carry;
        carry = !static_cast<digit_type>(*outputFirst + 1);
    }
}

}

template<typename DigitType>
Unsigned<DigitType> operator+(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto isLhsLess = lhs.magnitude() < rhs.magnitude();
    const auto& less    = isLhsLess ? lhs : rhs;
    const auto& greater = isLhsLess ? rhs : lhs;

    auto result = Unsigned(greater, greater.magnitude() + 1);
    details_::iteratorAdd(begin(result.digits_), begin(less.digits_), end(less.digits_));
    if (!result.digits_.back())
    {
        result.digits_.pop_back();
    }
    return result;
}

template<typename DigitType>
Unsigned<DigitType> operator-(const Unsigned<DigitType>& lhs, const Unsigned<DigitType>& rhs)
{
    const auto isLhsLess = lhs < rhs;
    const auto& less    = isLhsLess ? lhs : rhs;
    const auto& greater = isLhsLess ? rhs : lhs;

    auto result = greater;
    details_::iteratorDiff(begin(result.digits_), begin(less.digits_), end(less.digits_));
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
