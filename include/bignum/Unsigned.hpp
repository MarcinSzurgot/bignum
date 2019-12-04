#pragma once

#include "DigitSet.hpp"

#include <cstdint>
#include <vector>

namespace bignum
{

enum class Comparison {LT, EQ, GT};

struct Unsigned
{
    using size_type = typename DigitSet::size_type;

    Unsigned();

    Unsigned(digit_type digit)
    : digits_({digit})
    {

    }

    Unsigned(const DigitSet& digits)
    : digits_(digits)
    {

    }

    Unsigned(DigitSet&& digits)
    : digits_(std::move(digits))
    {

    }

    /**
     * Returns the number of digits. It is always greater than zero.
     * @return number of digits.
     */
    size_type magnitude() const { return digits_.size(); }

    digit_type digit(size_type index) const { return digits_[index]; }

    /**
     * @return the most significant digit.
     */
    digit_type msd() const { return digit(magnitude() - 1u); }

    /**
     * @return the least significant digit.
     */
    digit_type lsd() const { return digit(0); }

    explicit operator bool() const { return magnitude() > 1 || digit(0); }

    friend Comparison compare(const Unsigned& lhs, const Unsigned& rhs);

    friend bool operator==(const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator!=(const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator> (const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator< (const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator>=(const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator<=(const Unsigned& lhs, const Unsigned& rhs);

    friend Unsigned operator<<(const Unsigned& value, size_type offset);
    friend Unsigned operator>>(const Unsigned& value, size_type offset);

    friend Unsigned operator<<=(Unsigned& value, size_type offset) { return value = value << offset; }
    friend Unsigned operator>>=(Unsigned& value, size_type offset) { return value = value >> offset; }

    friend Unsigned operator+(const Unsigned& value);
    friend Unsigned operator-(const Unsigned& value);

private:
    DigitSet digits_;
};

}
