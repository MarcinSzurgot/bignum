#pragma once

#include "Comparison.hpp"

#include <algorithm>
#include <cwctype>
#include <iosfwd>
#include <string_view>
#include <type_traits>
#include <vector>

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
                (*this)[digit] = (value >> (bitSize * digit)) & ~DigitType();
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

    Unsigned(std::string_view string)
    {
        static const auto ten = Unsigned(digit_type(10));

        digits_.reserve(size(string));
        digits_.push_back(digit_type());

        auto tmp = Unsigned(digit_type());
        for (const auto c : string)
        {
            if (std::iswspace(c))
            {
                continue;
            }
            if (static_cast<bool>(*this))
            {
                (*this) *= ten;
            }
            tmp.lsd() = c - '0';
            *this += tmp;
        }
        trim();
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

    template<typename DigitType_> friend bool operator!(const Unsigned<DigitType_>& value)
    {
        return !static_cast<bool>(value);
    }

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

}

#include "UnsignedOperators/AddDiff.hpp"
#include "UnsignedOperators/Bitshift.hpp"
#include "UnsignedOperators/BitwiseLogic.hpp"
#include "UnsignedOperators/Comparison.hpp"
#include "UnsignedOperators/MulDiv.hpp"
#include "UnsignedOperators/Stream.hpp"
