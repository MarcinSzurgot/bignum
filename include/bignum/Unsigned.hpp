#pragma once

#include "Types.hpp"

#include <algorithm>
#include <cctype>
#include <cwctype>
#include <exception>
#include <iosfwd>
#include <string_view>
#include <type_traits>
#include <vector>

#include <climits>

namespace bignum
{

template<typename DigitType>
struct Unsigned
{
    static_assert(std::is_unsigned_v<DigitType>);

    using digit_type = DigitType;
    using size_type = typename std::vector<digit_type>::size_type;

    Unsigned()
    : Unsigned(DigitType())
    {

    }

    template<typename DigitType_,
             typename = std::enable_if_t<std::is_unsigned_v<DigitType_>>>
    Unsigned(DigitType_ value)
    : digits_(std::max(sizeof(DigitType_) / sizeof(DigitType), std::size_t(1)))
    {
        if (sizeof(DigitType_) <= sizeof(DigitType))
        {
            this->lsd() = value;
        }
        else
        {
            constexpr auto bitSize = sizeof(DigitType) * CHAR_BIT;
            for (auto digit = 0u; digit < magnitude(); ++digit)
            {
                (*this)[digit] = (value >> (bitSize * digit)) & DigitType(~DigitType());
            }
            trim();
        }
    }

    Unsigned(std::initializer_list<digit_type> list)
    : digits_(list)
    {
        trim();
    }

    Unsigned(const std::vector<digit_type>& digits)
    : digits_(digits)
    {
        trim();
    }

    Unsigned(std::vector<digit_type>&& digits)
    : digits_(std::move(digits))
    {
        trim();
    }

    /**
     * Converts string representation of Unsigned to actual Unsigned class object.
     * String should match "\s*\+?[0-9]" regex. It ignores leading whitespaces
     * and optional '+' sign on the beggining of digits string.
     * Any other character not matching to this pattern ends reading.
     * If reading was interrupted before any digit appeared then the final value of Unsigned is zero.
     * @param string is a string representation
     */
    Unsigned(std::string_view string)
    : Unsigned()
    {
        static const auto ten = Unsigned(10u);

        auto tmp = Unsigned(digit_type());
        auto first = begin(string), last = end(string);

        // Ignore leading whitespaces.
        first = std::find_if_not(first, last, std::iswspace);

        // Ignore '+' sign.
        if (*first == '+')
        {
            ++first;
        }

        // Ignore leading zeroes.
        first = std::find_if(first, last, [](const auto c){ return c != '0'; });

        // Find end of digits string.
        last = std::find_if_not(first, last, [](const auto c) { return std::isdigit(c); });

        if (first != last)
        {
            tmp.lsd() = DigitType(*first++ - '0');
            *this += tmp;
            for (; first != last; ++first)
            {
                tmp.lsd() = DigitType(*first - '0');
                *this = (*this) * ten + tmp;
            }
            trim();
        }
    }

    digit_type msd() const
    {
        return digits_.back();
    }

    digit_type lsd() const
    {
        return digits_.front();
    }

    size_type magnitude() const
    {
        return size(digits_);
    }

    digit_type operator[](size_type index) const
    {
        return digits_[index];
    }

    operator bool() const
    {
        return msd();
    }

    template<typename DigitType_> friend bool operator!(const Unsigned<DigitType_>& value);

    template<typename DigitType_> friend bool operator||(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator&&(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_> friend Comparison compare(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator< (const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator> (const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator<=(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator>=(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator==(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend bool operator!=(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_, typename Integer> friend Unsigned<DigitType_> operator<<(const Unsigned<DigitType_>& value, Integer offset);
    template<typename DigitType_, typename Integer> friend Unsigned<DigitType_> operator>>(const Unsigned<DigitType_>& value, Integer offset);

    template<typename DigitType_, typename Integer> friend Unsigned<DigitType_>& operator<<=(Unsigned<DigitType_>& value, Integer offset);
    template<typename DigitType_, typename Integer> friend Unsigned<DigitType_>& operator>>=(Unsigned<DigitType_>& value, Integer offset);

    template<typename DigitType_> friend Unsigned<DigitType_> operator+(Unsigned<DigitType_> value);
    template<typename DigitType_> friend Unsigned<DigitType_> operator-(Unsigned<DigitType_> value);
    template<typename DigitType_> friend Unsigned<DigitType_> operator+(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator-(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator*(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator/(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator%(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_> friend Unsigned<DigitType_>& operator+=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator-=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator*=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator/=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator%=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_> friend Unsigned<DigitType_> operator~(const Unsigned<DigitType_>& value);
    template<typename DigitType_> friend Unsigned<DigitType_> operator&(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator|(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator^(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_> friend Unsigned<DigitType_>& operator&=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator|=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator^=(Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_> friend std::ostream& operator<<(std::ostream& os, const Unsigned<DigitType_>& value);
    template<typename DigitType_> friend std::istream& operator>>(std::istream& is,       Unsigned<DigitType_>& value);

    template<typename DigitType_> friend Unsigned<DigitType_>& operator++(Unsigned<DigitType_>& value);
    template<typename DigitType_> friend Unsigned<DigitType_>& operator--(Unsigned<DigitType_>& value);
    template<typename DigitType_> friend Unsigned<DigitType_> operator++(Unsigned<DigitType_>& value, int);
    template<typename DigitType_> friend Unsigned<DigitType_> operator--(Unsigned<DigitType_>& value, int);

private:
    Unsigned(size_type size, digit_type sample)
    : digits_(size ? size : size_type(1), sample)
    {
    }

    Unsigned(const Unsigned& other, size_type newSize)
    : digits_(std::max(other.magnitude(), newSize))
    {
        std::copy(begin(other.digits_), end(other.digits_), begin(digits_));
    }

    void trim()
    {
        auto lastNonZero = size(digits_) - 1;
        for (; !digits_[lastNonZero] && lastNonZero > 0; --lastNonZero)
        {
        }
        digits_.resize(lastNonZero + 1);
    }

    digit_type& operator[](size_type index)
    {
        return digits_[index];
    }

    digit_type& msd()
    {
        return digits_.back();
    }

    digit_type& lsd()
    {
        return digits_.front();
    }

    std::vector<digit_type> digits_;
};

template<typename DigitType_> bool operator!(const Unsigned<DigitType_>& value)
{
    return !static_cast<bool>(value);
}

template<typename DigitType_> bool operator||(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs)
{
    return static_cast<bool>(lhs) || static_cast<bool>(rhs);
}

template<typename DigitType_> bool operator&&(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs)
{
    return static_cast<bool>(lhs) && static_cast<bool>(rhs);
}

template<typename DigitType_> Unsigned<DigitType_> operator+(Unsigned<DigitType_> value)
{
    return value;
}

template<typename DigitType_> Unsigned<DigitType_> operator-(Unsigned<DigitType_> value)
{
    return value;
}

template<typename DigitType_>
Unsigned<DigitType_>& operator++(Unsigned<DigitType_>& value)
{
    return value += Unsigned<DigitType_>(DigitType_(1));
}

template<typename DigitType_> 
Unsigned<DigitType_>& operator--(Unsigned<DigitType_>& value)
{
    return value -= Unsigned<DigitType_>(DigitType_(1));
}

template<typename DigitType_> 
Unsigned<DigitType_> operator++(Unsigned<DigitType_>& value, int)
{
    auto copy = value;
    ++value;
    return copy;
}

template<typename DigitType_> 
Unsigned<DigitType_> operator--(Unsigned<DigitType_>& value, int)
{
    auto copy = value;
    --value;
    return copy;
}

}

#include "UnsignedOperators/AddDiff.hpp"
#include "UnsignedOperators/Bitshift.hpp"
#include "UnsignedOperators/BitwiseLogic.hpp"
#include "UnsignedOperators/Comparison.hpp"
#include "UnsignedOperators/MulDiv.hpp"
#include "UnsignedOperators/Stream.hpp"
