#pragma once 

#include <concepts>

#include <bignum/BigUnsigned.hpp>
#include <bignum/Arrays/AdditiveOperations.hpp>
#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Arrays/Division.hpp>

namespace bignum {

auto divide(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> std::pair<BigUnsigned, BigUnsigned>;

auto operator+=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator-=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator/=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator%=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator+(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned;

auto operator-(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned;

auto operator*(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned;

auto operator/(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned;

auto operator%(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned;

}