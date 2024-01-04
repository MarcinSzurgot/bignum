#pragma once

#include <concepts>
#include <tuple>

#include <bignum/BigUnsigned.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<std::unsigned_integral U> 
struct Bits {
    static constexpr U Size = sizeof(U) * 8;
    static constexpr U Mask = ~U();
    static constexpr U HalfSize = Size / 2;
    static constexpr U HalfMask = Mask >> HalfSize;
    static constexpr U ShiftMask = Size - 1;
};

template<std::unsigned_integral U>
constexpr auto halve(U value) -> std::pair<U, U> {
    return {
         value                       & Bits<U>::HalfMask, 
        (value >> Bits<U>::HalfSize) & Bits<U>::HalfMask
    };
}

template<
    std::unsigned_integral U, 
    std::unsigned_integral Shift
>
constexpr auto lshift(U value, Shift shift) -> std::pair<U, U> {
    shift &= Bits<U>::ShiftMask;

    if (!shift) {
        return {value, U()};
    }

    return {
        value <<                  shift, // lower    
        value >> (Bits<U>::Size - shift) // higher   
    };
}

template<
    std::unsigned_integral U, 
    std::unsigned_integral Shift
>
constexpr auto rshift(U value, Shift shift) -> std::pair<U, U> {
    shift &= Bits<U>::ShiftMask;

    if (!shift) {
        return {U(), value};
    }

    return {
        value << (Bits<U>::Size - shift), // lower
        value >>                  shift   // higher
    };
}

template<std::unsigned_integral U> 
constexpr auto add(U lhs, U rhs) -> std::pair<U, U> {
    const auto result = U(lhs + rhs);
    return {result, result < lhs};
}

template<std::unsigned_integral U> 
constexpr auto sub(U lhs, U rhs) -> std::pair<U, U> {
    const auto result = U(lhs - rhs);
    return {result, result > lhs};
}

template<std::unsigned_integral U>
constexpr auto mul(U lhs, U rhs) -> std::pair<U, U> {
    const auto [lowerLhs, upperLhs] = halve(lhs);
    const auto [lowerRhs, upperRhs] = halve(rhs);

    const auto [middleResult, middleCarry] = add<U>(upperLhs * lowerRhs, lowerLhs * upperRhs);
    const auto [ lowerResult,  lowerCarry] = add<U>(lowerLhs * lowerRhs, (middleResult << Bits<U>::HalfSize));

    const auto upperResult = U(
        upperLhs * upperRhs
        + (middleCarry  << Bits<U>::HalfSize)
        + (middleResult >> Bits<U>::HalfSize) 
        + lowerCarry
    );

    return {lowerResult, upperResult};
}

template<std::unsigned_integral U>
constexpr auto div(U lhs, U rhs) -> std::pair<U, U> {
    const auto d = U(lhs / rhs);
    return {d, lhs - rhs * d};
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