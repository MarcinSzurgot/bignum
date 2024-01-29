#pragma once

#include <concepts>
#include <span>

#include <bignum/Concepts/Concepts.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Additive.hpp>

namespace bignum {

template<
    typename InputRange,
    typename OutputIter,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIter>
>
requires TransformableToIter<OutputIter, 
    std::remove_reference_t<InputRange>,
    std::remove_reference_t<Unsigned>
>
auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    OutputIter result
) -> Unsigned {
    return cascade(lhs, Unsigned(), result, 
        [rhs](auto next, auto carry){
            const auto [lower,     higher] = mul(next,    rhs);
            const auto [result, nextCarry] = add(lower, carry);
            return std::make_pair(result, nextCarry + higher);
    });
}

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator InputIterator,
    std::forward_iterator OutputIter,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIter>
>
auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    InputIterator adding,
    OutputIter result
) -> std::pair<Unsigned, OutputIter> {
    auto carry = cascade(lhs, Unsigned(), result, 
        [rhs, &result](auto&& next1, auto&& next2, auto&& carry){
            ++result;
            const auto [lower,     higher] = mul(next1,   rhs);
            const auto [result, nextCarry] = add(lower, next2, carry);
            return std::make_pair(result, nextCarry + higher);
    }, adding);

    for (; carry; ++result) {
        std::tie(*result, carry) = add(*result, carry);
    }

    return std::make_pair(carry, result);
}

template<
    std::ranges::forward_range InputRange1, 
    std::ranges::forward_range InputRange2, 
    std::forward_iterator OutputIterator
>
auto mul(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputIterator output
) -> void {
    for (auto&& r : rhs) {
        mul(lhs, r, output, output);
        ++output;
    }
}

}

