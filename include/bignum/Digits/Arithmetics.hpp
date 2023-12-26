#pragma once

#include <concepts>

#include <bignum/BigUnsigned.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<std::unsigned_integral U> 
auto add(U lhs, U rhs) -> std::pair<U, U> {
    const auto result = lhs + rhs;
    return {result, result < lhs};
}

template<std::unsigned_integral U> 
auto sub(U lhs, U rhs) -> std::pair<U, U> {
    const auto result = lhs - rhs;
    return {result, result > lhs};
}

template<std::unsigned_integral U>
auto mul(U lhs, U rhs) -> std::pair<U, U> {
    static constexpr auto halfBitSize = sizeof(U) * 8 / 2;
    static constexpr auto halfMask = (U(1) << halfBitSize) - 1;

    const U lowerLeft  =  lhs                 & halfMask;
    const U lowerRight =  rhs                 & halfMask;
    const U upperLeft  = (lhs >> halfBitSize) & halfMask;
    const U upperRight = (rhs >> halfBitSize) & halfMask;

    const U lowerResult      = lowerLeft * lowerRight;
    const U middleResultPart = lowerLeft * upperRight;
    const U middleResult     = middleResultPart + lowerRight * upperLeft;
    const U upperResult      = upperLeft * upperRight + (
        middleResult < middleResultPart 
        ? (U(1) << halfBitSize)
        :  U(0)
    );

    const U finalLowerResult = lowerResult + (middleResult << halfBitSize);
    const U finalUpperResult = upperResult + (middleResult >> halfBitSize) + (
        finalLowerResult < lowerResult 
        ? U(1)
        : U(0)
    );

    return {finalLowerResult, finalUpperResult};
}

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

auto operator+(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned;

auto operator*(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned;

}