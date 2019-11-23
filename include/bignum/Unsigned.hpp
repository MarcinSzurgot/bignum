#pragma once

#include <cstdint>
#include <vector>

namespace bignum
{

using digit_type = std::uint8_t;

enum class Comparison {LT, EQ, GT};

struct Unsigned
{
    using size_type = typename std::vector<digit_type>::size_type;

    Unsigned();
    Unsigned(std::vector<digit_type> digits);

    size_type magnitude() const { return digits_.size(); }
    digit_type digit(size_type index) const { return digits_[index]; }

    Comparison compare(const Unsigned& other) const;

private:
    std::vector<digit_type> digits_;
};

}
