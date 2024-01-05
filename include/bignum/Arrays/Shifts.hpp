#pragma once

#include <concepts>
#include <span>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Digits/Arithmetics.hpp>

namespace bignum {

template<
    typename U, 
    typename K
>
requires std::unsigned_integral<std::remove_const_t<U>>
    && std::unsigned_integral<K>
    && std::same_as<std::remove_const_t<U>, K>
auto rshift(
    std::span<U> source,
    std::size_t bitShift,
    std::span<K> result
) -> K {
    bitShift &= Bits<U>::ShiftMask;

    auto [outShifted, carry] = rshift(source[0], bitShift);
    for (auto d = typename std::span<U>::size_type(1); d < size(source); ++d) {
        const auto [lower, upper]      = rshift(source[d], bitShift);
        std::tie(result[d - 1], carry) = std::make_pair(carry | lower, upper);
    }

    result[size(source) - 1] = carry;

    return outShifted;
}

template<
    typename U, 
    typename K
>
requires std::unsigned_integral<std::remove_const_t<U>>
    && std::unsigned_integral<K>
    && std::same_as<std::remove_const_t<U>, K>
auto lshift(
    std::span<U> source,
    std::size_t bitShift,
    std::span<K> result
) -> K {
    bitShift &= Bits<U>::ShiftMask;

    auto carry = U();

    for (auto d = typename std::span<U>::size_type(); d < size(source); ++d) {
        const auto [lower, upper] = lshift(source[d], bitShift);
        std::tie(result[d], carry) = std::make_pair(carry | lower, upper);
    }

    return carry;
}

}