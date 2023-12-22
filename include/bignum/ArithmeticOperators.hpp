#pragma once 

#include <concepts>

#include <bignum/BigUnsigned.hpp>
#include <bignum/ArrayArithmetics/ArraySubAdd.hpp>
#include <bignum/ArrayArithmetics/ArrayMul.hpp>
#include <bignum/ArrayArithmetics/ArrayDiv.hpp>

namespace bignum {

auto divide(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> std::pair<BigUnsigned, BigUnsigned>;

auto operator+=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator+=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

template<std::unsigned_integral LessThanNative>
requires (sizeof(LessThanNative) < sizeof(BigUnsigned::NativeDigit)) 
auto operator+=(
    BigUnsigned& lhs,
    LessThanNative rhs
) -> BigUnsigned& {
    return lhs += BigUnsigned::NativeDigit(rhs);
}

auto operator-=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator-=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

template<std::unsigned_integral LessThanNative>
requires (sizeof(LessThanNative) < sizeof(BigUnsigned::NativeDigit)) 
auto operator-=(
    BigUnsigned& lhs,
    LessThanNative rhs
) -> BigUnsigned& {
    return lhs -= BigUnsigned::NativeDigit(rhs);
}

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned&;

auto operator*=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

template<std::unsigned_integral LessThanNative>
requires (sizeof(LessThanNative) < sizeof(BigUnsigned::NativeDigit)) 
auto operator*=(
    BigUnsigned& lhs,
    LessThanNative rhs
) -> BigUnsigned& {
    return lhs *= BigUnsigned::NativeDigit(rhs);
}

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