#pragma once

#include <iosfwd>
#include <type_traits>
#include <vector>

namespace bignum
{

enum struct Comparison
{
    LT,
    EQ,
    GT
};

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

    Unsigned(DigitType digit)
    : digits_{digit}
    {

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

    template<typename DigitType_> friend Unsigned<DigitType_> operator&(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator|(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);
    template<typename DigitType_> friend Unsigned<DigitType_> operator^(const Unsigned<DigitType_>& lhs, const Unsigned<DigitType_>& rhs);

    template<typename DigitType_> friend std::ostream& operator<<(std::ostream& os, const Unsigned<DigitType_>& value);
    template<typename DigitType_> friend std::istream& operator>>(std::istream& is,       Unsigned<DigitType_>& value);

private:
    Unsigned(size_type size, digit_type sample)
    : digits_(size, sample)
    {

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

    std::vector<digit_type> digits_;
};

}

#include "Operators/AddDiff.hpp"
#include "Operators/Bitshift.hpp"
#include "Operators/BitwiseLogic.hpp"
#include "Operators/Comparison.hpp"
#include "Operators/MulDiv.hpp"
#include "Operators/Stream.hpp"
