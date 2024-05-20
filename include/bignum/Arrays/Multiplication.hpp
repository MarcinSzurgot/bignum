#pragma once

#include <concepts>
#include <span>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Additive.hpp>

namespace bignum {

// template<
//     std::input_iterator Result,
//     std::ranges::input_range    Range,
//     std::ranges::input_range ...Ranges
// > requires std::unsigned_integral<std::iter_value_t<Result>>
//     &&  std::same_as<std::iter_value_t<Result>, std::ranges::range_value_t<Range>>
//     && (std::same_as<std::iter_value_t<Result>, std::ranges::range_value_t<Ranges>> && ...)
// using MultiplicativeResult = CascadeResult<
//     std::iter_value_t<Result>,
//     Result,
//     std::ranges::iterator_t<Range>,
//     std::ranges::iterator_t<Ranges>...
// >;

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    OutputIterator result
) -> CascadeResult<
    Unsigned, 
    OutputIterator,
    std::ranges::iterator_t<InputRange>
> {
    return cascade(lhs, Unsigned(), result,
        [rhs](auto left, auto carry){
            const auto [lower,     higher] = mul(left,    rhs);
            const auto [result, leftCarry] = add(lower, carry);
            return std::make_pair(result, leftCarry + higher);
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
) -> CascadeResult<
    Unsigned, 
    OutputIterator,
    std::ranges::iterator_t<InputRange>,
    InputIterator
> {
    return cascade(lhs, Unsigned(), result, 
        [rhs](auto&& next1, auto&& next2, auto&& carry){
            const auto [lower,     higher] = mul(next1,   rhs);
            const auto [result, nextCarry] = add(lower, next2, carry);
            return std::make_pair(result, nextCarry + higher);
    }, adding);
}

template<
    std::ranges::input_range Minuend, 
    std::ranges::input_range Subtrahend, 
    std::unsigned_integral Unsigned,
    std::input_iterator Result
> requires
    std::same_as<Unsigned, std::iter_value_t<Result>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Subtrahend>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Minuend>>
constexpr auto mulSub(
    Minuend&& minuend,
    Subtrahend&& subtrahend,
    Unsigned multiplier,
    Result result
) -> void {
    auto minuendFirst = begin(minuend);

    auto carry = Unsigned();
    for (auto s : subtrahend) {
        const auto [lower, higher] = mul(s, multiplier);
        std::tie(*result, carry) = sub(*minuendFirst, lower, carry);
        carry += higher;

        ++result;
        ++minuendFirst;
    }

    for (auto minuendLast = end(minuend); minuendFirst != minuendLast;) {
        std::tie(*result, carry) = sub(*minuendFirst, carry);

        ++result;
        ++minuendFirst;
    }
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
        auto [
            carry, 
            continuation,
            _
        ] = mul(lhs, r, output, output);

        for (; carry; ++continuation) {
            std::tie(*continuation, carry) = add(*continuation, carry);
        }

        ++output;
    }
}

}

