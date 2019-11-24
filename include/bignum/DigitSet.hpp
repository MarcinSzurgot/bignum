#pragma once

#include "Defines.hpp"

#include <vector>

namespace bignum
{

struct DigitSet
{
    using size_type = std::vector<digit_type>::size_type;

    DigitSet();
    DigitSet(const std::vector<digit_type>& digits);
    DigitSet(std::initializer_list<digit_type> list);

    void trim();

    size_type size() const { return digits_.size(); }

    digit_type operator[](size_type index) const { return digits_[index]; }
    digit_type& operator[](size_type index) { return digits_[index]; }

private:
    std::vector<digit_type> digits_;
};

}
