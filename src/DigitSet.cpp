#include "DigitSet.hpp"

#include <algorithm>

namespace bignum
{

DigitSet::DigitSet()
: DigitSet({0})
{

}

DigitSet::DigitSet(const std::vector<digit_type>& digits)
: digits_(digits.empty() ? std::vector<digit_type>{digit_type()} : digits)
{
    trim();
}

DigitSet::DigitSet(std::initializer_list<digit_type> list)
: digits_(std::vector<digit_type>(list))
{

}

DigitSet::DigitSet(size_type size, const digit_type& sample)
: digits_(size ? size : 1u, sample)
{

}

void DigitSet::trim()
{
    auto lastNonZeroIndex = size_type();
    for(auto digit = digits_.size(); digit > 0u; --digit)
    {
        if (digits_[digit - 1])
        {
            lastNonZeroIndex = digit - 1;
            break;
        }
    }
    digits_.resize(lastNonZeroIndex + 1);
}

void DigitSet::push_front(digit_type digit)
{
    digits_.insert(begin(digits_), digit);
}

void DigitSet::push_back(digit_type digit)
{
    digits_.push_back(digit);
}

void DigitSet::pop_back()
{
    if (digits_.size() > size_type(1))
    {
        digits_.pop_back();
    }
}

void DigitSet::pop_front()
{
    if (digits_.size() > size_type(1))
    {
        digits_.erase(begin(digits_));
    }
}

void DigitSet::clear()
{
    *this = {};
}

void DigitSet::resize(size_type size)
{
    digits_.resize(size ? size : 1);
}

bool operator==(const DigitSet& lhs, const DigitSet& rhs)
{
    return lhs.digits_ == rhs.digits_;
}

bool operator!=(const DigitSet& lhs, const DigitSet& rhs)
{
    return !(lhs == rhs);
}

}
