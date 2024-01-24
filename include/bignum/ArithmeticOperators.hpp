#pragma once 

#include <concepts>

#include <bignum/BigUnsigned.hpp>
#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Arrays/Division.hpp>
#include <bignum/Ranges/Additive.hpp>

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

auto operator+=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

auto operator-=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

auto operator*=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

auto operator/=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned&;

inline auto operator+(
    BigUnsigned&& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    lhs += rhs;
    return std::move(lhs);
}

inline auto operator+(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    return BigUnsigned(lhs) + rhs;
}

inline auto operator-(
    BigUnsigned&& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    lhs -= rhs;
    return std::move(lhs);
}

inline auto operator-(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    return BigUnsigned(lhs) - rhs;
}

inline auto operator*(
    BigUnsigned&& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    lhs *= rhs;
    return std::move(lhs);
}

inline auto operator*(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    return BigUnsigned(lhs) * rhs;
}

}