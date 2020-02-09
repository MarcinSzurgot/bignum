#pragma once

#include "Unsigned.hpp"

namespace bignum
{

enum class Sign
{
    Minus,
    Plus
};

template<typename DigitType>
struct Integer
{
    static_assert(std::is_unsigned_v<DigitType>);

    using digit_type    = DigitType;
    using unsigned_type = Unsigned<digit_type>;

    Integer()
    : unsigned_(),
      sign_(Sign::Plus)
    {

    }

    Integer(Sign sign, const Unsigned<DigitType>& abs)
    : unsigned_(abs),
      sign_(sign)
    {

    }

    Integer(Sign sign, Unsigned<DigitType>&& abs)
    : unsigned_(std::move(abs)),
      sign_(sign)
    {

    }

    template<typename DigitType_,
             typename = std::enable_if_t<std::is_integral_v<DigitType_>>>
    Integer(DigitType_ digit)
    : unsigned_(DigitType(digit < DigitType_() ? -digit : digit)),
      sign_(digit < DigitType_() ? Sign::Minus : Sign::Plus)
    {

    }

    Sign sign() const
    {
        return sign_;
    }

    const unsigned_type& abs() const
    {
        return unsigned_;
    }

    template<typename DigitType_>
    friend unsigned_type abs(const Integer& value)
    {
        return value.abs();
    }

    template<typename DigitType_> friend Comparison compare(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator <(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator >(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator<=(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator>=(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator==(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator!=(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);

private:
    unsigned_type unsigned_;
    Sign sign_;
};

}

#include "IntegerOperators/Comparison.hpp"
