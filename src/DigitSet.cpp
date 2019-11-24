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

}
