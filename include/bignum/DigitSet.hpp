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
    DigitSet(size_type size, const digit_type& sample = {});

    void trim();
    void push_front(digit_type digit);
    void push_back(digit_type digit);
    void pop_back();
    void pop_front();
    void clear();
    void resize(size_type size);

    size_type size() const { return digits_.size(); }

    digit_type operator[](size_type index) const { return digits_[index]; }
    digit_type& operator[](size_type index) { return digits_[index]; }

    friend bool operator==(const DigitSet& lhs, const DigitSet& rhs);
    friend bool operator!=(const DigitSet& lhs, const DigitSet& rhs);

private:
    std::vector<digit_type> digits_;
};

}
