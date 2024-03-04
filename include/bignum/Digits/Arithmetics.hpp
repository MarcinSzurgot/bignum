#pragma once

#include <iostream>
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

template<
    std::unsigned_integral Unsigned,
    std::unsigned_integral ...Us>
requires (std::same_as<Unsigned, Us> && ...)
constexpr auto add(
    Unsigned a,
    Unsigned b, 
    Us... rest
) -> std::pair<Unsigned, Unsigned> {
    if constexpr (sizeof...(Us) == 0) {
        const auto result = Unsigned(a + b);
        return {result, result < a};
    } else {
        const auto& [ firstResult,  firstCarry] = add(a, b);
        const auto& [secondResult, secondCarry] = add(firstResult, rest...);

        return std::make_pair(secondResult, firstCarry + secondCarry);
    }
}

template<
    std::unsigned_integral Unsigned,
    std::unsigned_integral ...Us>
requires (std::same_as<Unsigned, Us> && ...)
constexpr auto sub(
    Unsigned a,
    Unsigned b, 
    Us... rest
) -> std::pair<Unsigned, Unsigned> {
    if constexpr (sizeof...(Us) == 0) {
        const auto result = Unsigned(a - b);
        return {result, result > a};
    } else {
        const auto& [ firstResult,  firstCarry] = add(b, rest...);
        const auto& [secondResult, secondCarry] = sub(a, firstResult);

        return std::make_pair(secondResult, firstCarry + secondCarry);
    }
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

template<std::unsigned_integral U>
constexpr auto div(
    U lower,
    U higher,
    U divisor
) -> std::pair<std::array<U, 2>, U> {
    const auto base = Bits<U>::Mask;
    const auto [bq, br] = div(base, divisor);

    auto [higherQuotient, higherRemaining] = div(higher, divisor);
    auto lowerQuotientFinal = U();

    while(higherRemaining) {
        const auto [lowerQuotient, lowerRemainder] = div(lower, divisor);
        const auto [lowerRemainingNextPartial, higherRemainingNext] = mul<U>(br + 1, higherRemaining);
        const auto [lowerRemainingNext, carry] = add(lowerRemainingNextPartial, lowerRemainder);

        lowerQuotientFinal += bq * higherRemaining + lowerQuotient;
        lower = lowerRemainingNext;
        higherRemaining = higherRemainingNext + carry;
    }

    const auto [lastQuotient, lastRemainder] = div(lower, divisor);

    lowerQuotientFinal += lastQuotient;

    return std::make_pair(
        std::array {
            lowerQuotientFinal, 
            higherQuotient
        }, lastRemainder
    );
}

}