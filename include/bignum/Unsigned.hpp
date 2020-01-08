#pragma once

#include "DigitSet.hpp"

#include <iostream>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <vector>

namespace bignum
{

enum class Comparison {LT, EQ, GT};

template<typename DigitType>
struct Unsigned
{
    using digit_type = IntWrapper<DigitType>;
    using digit_set = DigitSet<digit_type>;
    using size_type = typename digit_set::size_type;

    Unsigned() = default;

    Unsigned(digit_type digit)
    : digits_(digit_set(digit))
    {

    }

    Unsigned(std::initializer_list<digit_type> digits)
    : digits_(digits)
    {

    }

    Unsigned(const digit_set& digits)
    : digits_(digits)
    {

    }

    Unsigned(digit_set&& digits)
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

    template
    <
        typename Float,
        typename = typename std::enable_if<std::is_floating_point_v<Float>>::type
    >
    explicit operator Float() const
    {
        constexpr auto order = digit_type::max().value + Float(1);

        return std::accumulate
        (
            next(begin(digits_)),
            end(digits_),
            static_cast<Float>(*begin(digits_)),
            [](auto s, auto d) { return s * order + d.value; }
        );
    }

    explicit operator digit_type() const { return msd(); }

    explicit operator bool() const { return static_cast<bool>(msd()); }

    friend bool operator!(const Unsigned& value) { return !static_cast<bool>(value); }

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

    friend Unsigned operator+(const Unsigned& lhs, const Unsigned& rhs);
    friend Unsigned operator-(const Unsigned& lhs, const Unsigned& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Unsigned& value);

private:
    digit_set digits_;
};

}

#include "Operators/UnsignedComparisonOperators.hpp"
