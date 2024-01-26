#pragma once

#include <concepts>
#include <span>

#include <bignum/Concepts/Concepts.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Additive.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<
    typename InputRange, 
    typename OutputIter, 
    std::unsigned_integral Unsigned = std::ranges::range_value_t<InputRange>
>
requires TransformableToIter<OutputIter, InputRange, Unsigned>
auto mul(
    InputRange&& lhs, 
    Unsigned rhs,
    OutputIter&& result
) -> Unsigned {
    return transformWithCarry(lhs, Unsigned(), result, 
        [rhs](auto next, auto carry){
            const auto [lower,     higher] = mul(next,    rhs);
            const auto [result, nextCarry] = add(lower, carry);
            return std::make_pair(result, nextCarry + higher);
    });
}

template<
    typename T, 
    typename U, 
    typename K
> requires
    std::unsigned_integral<std::remove_const_t<T>>
    && std::unsigned_integral<std::remove_const_t<U>>
    && std::unsigned_integral<K>
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
    && std::same_as<std::remove_const_t<U>, K>
auto mul(
    std::span<T> lhs,
    std::span<U> rhs,
    std::span<K> result
) -> void {
    auto resultLast = end(result);

    // for (auto r = typename std::span<T>::size_type(); r < size(rhs); ++r) {
        // const auto [lower, higher] = mul(lhs[l], rhs[r]);

        // std::tie(* resultIter     , carry) = add(* resultIter     ,         lower);
        // std::tie(*(resultIter + 1), carry) = add(*(resultIter + 1), higher, carry);

        // const auto carry = mul(lhs, rhs[r], result.subspan(r).begin());
        // add(result.subspan(r + size(lhs)), carry, result.subspan(r + size(lhs)));
    // }

    for (auto l = typename std::span<T>::size_type(); l < size(lhs); ++l) {
        auto carry = K();
        auto resultIter = std::next(begin(result), l);

        for (auto r = typename std::span<T>::size_type(); r < size(rhs); ++r, ++resultIter) {
            const auto [lower, higher] = mul(lhs[l], rhs[r]);

            std::tie(* resultIter     , carry) = add(* resultIter     ,         lower);
            std::tie(*(resultIter + 1), carry) = add(*(resultIter + 1), higher, carry);

            add(
                std::ranges::subrange(resultIter + 2, resultLast), 
                carry, 
                std::ranges::subrange(resultIter + 2, resultLast)
            );
        }
    }
}

}

