#pragma once

#include "Unsigned.hpp"

#include <algorithm>
#include <iosfwd>
#include <iostream>

namespace bignum
{

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

    Integer(std::string_view string)
    {
        auto first = begin(string);
        auto last = end(string);

        first = std::find_if_not(first, last, std::iswspace);
        if (*first == '-')
        {
            sign_ = Sign::Minus;

            // Unsigned constructor would accept plus sign at the beggining
            // but it makes no sense to have it right after minus.
            if (*++first == '+')
            {
                sign_ = Sign::Plus;
                return;
            }
        }
        else
        {
            sign_ = Sign::Plus;
        }

        const auto rest = std::string_view
        (
            string.data() + std::distance(begin(string), first),
            std::distance(first, last)
        );
        unsigned_ = Unsigned<DigitType>(rest);

        if (sign_ == Sign::Minus && !unsigned_)
        {
            sign_ = Sign::Plus;
        }
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

    template<typename DigitType_> friend Integer<DigitType_> operator+(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend Integer<DigitType_> operator-(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend Integer<DigitType_> operator*(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend Integer<DigitType_> operator/(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);
    template<typename DigitType_> friend Integer<DigitType_> operator%(const Integer<DigitType_>& lhs, const Integer<DigitType_>& rhs);

    template<typename DigitType_> friend std::ostream& operator<<(std::ostream& os, const Integer<DigitType_>& value);
    template<typename DigitType_> friend std::istream& operator>>(std::istream& is,       Integer<DigitType_>& value);

private:
    unsigned_type unsigned_;
    Sign sign_;
};

}

#include "IntegerOperators/AddDiff.hpp"
#include "IntegerOperators/Comparison.hpp"
#include "IntegerOperators/MulDiv.hpp"
#include "IntegerOperators/Stream.hpp"
