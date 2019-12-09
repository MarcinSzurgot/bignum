#pragma once

#include "Defines.hpp"

#include <utility>

namespace bignum
{

inline std::pair<digit_type, digit_type> addWithCarry(digit_type lhs, digit_type rhs, digit_type carry)
{
    return {lhs + rhs + carry, digit_type()};
}

}
