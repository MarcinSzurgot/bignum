#pragma once

#include "NonEmptyVector.hpp"

#include <algorithm>
#include <iosfwd>
#include <iterator>

namespace bignum
{

template<typename DigitType>
struct DigitSet
{
    static_assert(std::is_integral_v<DigitType>);

    using digit_type = DigitType;
    using size_type = typename NonEmptyVector<digit_type>::size_type;
    using const_reference = typename NonEmptyVector<digit_type>::const_reference;

    DigitSet()
    : digits_()
    {

    }

    DigitSet(size_type size, digit_type sample = {})
    : digits_(sample ? size : size_type(1), sample)
    {

    }

    DigitSet(std::initializer_list<digit_type> list)
    : digits_(list)
    {
        trim();
    }

    DigitSet(const NonEmptyVector<digit_type>& digits)
    : digits_(digits)
    {
        trim();
    }

    DigitSet(NonEmptyVector<digit_type>&& digits)
    : digits_(std::move(digits))
    {
        trim();
    }

    template<typename Operator>
    void edit(Operator&& op)
    {
        op(digits_);
        trim();
    }

    const_reference operator[](size_type index) const { return digits_[index]; }

    friend bool empty(const DigitSet& digitSet) { return empty(digitSet.digits_); }
    friend size_type size(const DigitSet& digitSet) { return size(digitSet.digits_); }

    friend auto begin(DigitSet& digitSet) { return begin(digitSet.digits_); }
    friend auto   end(DigitSet& digitSet) { return   end(digitSet.digits_); }

    friend auto begin(const DigitSet& digitSet) { return begin(digitSet.digits_); }
    friend auto   end(const DigitSet& digitSet) { return   end(digitSet.digits_); }

    friend auto rbegin(DigitSet& digitSet) { return rbegin(digitSet.digits_); }
    friend auto   rend(DigitSet& digitSet) { return   rend(digitSet.digits_); }

    friend auto rbegin(const DigitSet& digitSet) { return rbegin(digitSet.digits_); }
    friend auto   rend(const DigitSet& digitSet) { return   rend(digitSet.digits_); }

private:
    void trim()
    {
        const auto lastNonZeroReverse = std::find_if
        (
            rbegin(digits_),
            rend(digits_),
            [](digit_type value)
            {
                return value != digit_type();
            }
        );
        const auto lastNonZero = std::next(lastNonZeroReverse).base();
        const auto firstLeadingZero = std::next(lastNonZero);

        digits_.erase(firstLeadingZero, end(digits_));
    }

    NonEmptyVector<digit_type> digits_;
};

}
