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
    typename InputRange1,
    typename InputRange2,
    typename OutputIter, 
    typename BinaryOp,
    std::unsigned_integral Unsigned = std::iter_value_t<std::remove_reference_t<OutputIter>>
>
requires TransformableToIter<
    std::remove_reference_t<OutputIter>, 
    std::remove_reference_t<InputRange1>,
    std::remove_reference_t<InputRange2>
>
constexpr auto transformWithCarry(
    InputRange1&& input1,
    InputRange2&& input2,
    Unsigned initialCarry,
    OutputIter&& out,
    BinaryOp&& op  
) -> std::pair<Unsigned, OutputIter> {
    for(auto&& [i1, i2] : std::views::zip(input1, input2)) {
        std::tie(*out++, initialCarry) = op(i1, i2, initialCarry);
    }
    return {initialCarry, out};
}

template<
    typename InputRange,
    typename OutputIter,
    std::unsigned_integral Unsigned = std::iter_value_t<std::remove_reference_t<OutputIter>>
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
    return transformWithCarry(lhs, Unsigned(), result, 
        [rhs](auto next, auto carry){
            const auto [lower,     higher] = mul(next,    rhs);
            const auto [result, nextCarry] = add(lower, carry);
            return std::make_pair(result, nextCarry + higher);
    });
}

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputIter,
    std::unsigned_integral Unsigned = std::iter_value_t<std::remove_reference_t<OutputIter>>
>
requires TransformableToIter<OutputIter, 
    std::remove_reference_t<InputRange1>,
    std::remove_reference_t<InputRange2>,
    std::remove_reference_t<Unsigned>
>
auto mul(
    InputRange1&& lhs,
    Unsigned rhs,
    InputRange2&& adding,
    OutputIter result
) -> std::pair<Unsigned, OutputIter> {
    return transformWithCarry(lhs, adding, Unsigned(), result, 
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
> requires TransformableToRange<
    std::remove_reference_t<OutputRange>,
    std::remove_reference_t<InputRange1>,
    std::remove_reference_t<InputRange2>
>
auto mul(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> void {
    auto firstRhs    = begin(rhs);
    auto lastRhs     = end(rhs);
    auto firstResult = begin(result);
    auto lastResult  = end(result);

    for (; firstRhs != lastRhs; ++firstRhs, ++firstResult) {
        const auto [carry, afterMulResult] = mul(
            lhs, 
            *firstRhs, 
            std::ranges::subrange(firstResult, lastResult),
            firstResult
        );

        auto finishResult = std::ranges::subrange(afterMulResult, lastResult);

        add(finishResult, carry, finishResult);
    }
}

}

