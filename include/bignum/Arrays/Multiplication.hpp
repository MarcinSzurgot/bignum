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
    }).first;
}

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputIter,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIter>
>
auto mul(
    InputRange1&& lhs,
    Unsigned rhs,
    InputRange2&& adding,
    OutputIter result
) -> std::pair<Unsigned, OutputIter> {
    return cascade(lhs, adding, Unsigned(), result, 
        [rhs](auto&& next1, auto&& next2, auto&& carry){
            const auto [lower,     higher] = mul(next1,   rhs);
            const auto [result, nextCarry] = add(lower, next2, carry);
            return std::make_pair(result, nextCarry + higher);
    });
}

template<
    std::ranges::input_range InputRange1, 
    std::ranges::input_range InputRange2, 
    typename OutputRange
>
auto mul(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> void {
    auto firstResult = begin(result);
    auto lastResult  = end(result);

    for (auto&& r : rhs) {
        const auto [carry, afterMulResult] = mul(
            lhs, r, std::ranges::subrange(firstResult, lastResult), firstResult
        );

        ++firstResult;

        auto finishResult = std::ranges::subrange(afterMulResult, lastResult);

        add(finishResult, carry, afterMulResult);
    }
}

}

