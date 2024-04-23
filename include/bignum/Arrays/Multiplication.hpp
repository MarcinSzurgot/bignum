#pragma once

#include <concepts>
#include <span>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Additive.hpp>

namespace bignum {

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    OutputIterator result
) -> Unsigned {
    return cascade(lhs, Unsigned(), result, 
        [rhs, &result](auto next, auto carry){
            ++result;
            const auto [lower,     higher] = mul(next,    rhs);
            const auto [result, nextCarry] = add(lower, carry);
            return std::make_pair(result, nextCarry + higher);
    });
}

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator InputIterator,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    InputIterator adding,
    OutputIterator result
) -> std::pair<Unsigned, OutputIterator> {
    auto carry = cascade(lhs, Unsigned(), result, 
        [rhs, &result](auto&& next1, auto&& next2, auto&& carry){
            ++result;
            const auto [lower,     higher] = mul(next1,   rhs);
            const auto [result, nextCarry] = add(lower, next2, carry);
            return std::make_pair(result, nextCarry + higher);
    }, adding);

    return std::make_pair(carry, result);
}

template<
    std::ranges::forward_range InputRange1, 
    std::ranges::forward_range InputRange2, 
    std::forward_iterator OutputIterator
>
constexpr auto mul(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputIterator output
) -> void {
    for (auto&& r : rhs) {
        auto [carry, continuation] = mul(lhs, r, output, output);
        ++output;

        for (; carry; ++continuation) {
            std::tie(*continuation, carry) = add(*continuation, carry);
        }
    }
}

}

