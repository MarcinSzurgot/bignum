#pragma once

#include "BigUnsigned.hpp"

namespace bignum {

auto operator<<(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned;

auto operator>>(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned;

auto operator<<=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

auto operator>>=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

}