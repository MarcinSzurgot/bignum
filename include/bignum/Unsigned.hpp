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
    Unsigned(const DigitSet& digits);

    /**
     * Returns the number of digits. It is always greater than zero.
     * @return number of digits.
     */
    size_type magnitude() const { return digits_.size(); }

    digit_type digit(size_type index) const { return digits_[index]; }

    operator bool() const { return magnitude() > 1 || digit(0); }

    friend Comparison compare(const Unsigned& lhs, const Unsigned& rhs);

    friend bool operator==(const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator!=(const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator> (const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator< (const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator>=(const Unsigned& lhs, const Unsigned& rhs);
    friend bool operator<=(const Unsigned& lhs, const Unsigned& rhs);

private:
    DigitSet digits_;
};

}
