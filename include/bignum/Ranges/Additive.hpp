#pragma once 

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Concepts/Concepts.hpp>
#include <bignum/Ranges/Algorithms.hpp>

#include <algorithm>
#include <concepts>
#include <numeric>
#include <ranges>
#include <span>  
#include <tuple>

namespace bignum {

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<OutputRange>
>
requires TransformableToRange<OutputRange, InputRange1, InputRange2>
constexpr auto sub(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> std::ranges::range_value_t<OutputRange> { 
    return transformWithCarry(lhs, rhs, lhs, sub<std::ranges::range_value_t<OutputRange>>); 
}

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<OutputRange>
> 
requires TransformableToRange<OutputRange, InputRange1, InputRange2>
constexpr auto add(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> Unsigned { 
    return transformWithCarry(lhs, rhs, result, add<Unsigned>); 
}

template<
    typename InputRange,
    typename OutputRange, 
    std::unsigned_integral Unsigned
> 
requires TransformableToRange<OutputRange, InputRange, Unsigned>
constexpr auto sub(
    InputRange&& lhs, 
    Unsigned rhs,
    OutputRange&& result
) -> Unsigned { 
    return sub(
        std::forward<InputRange>(lhs), 
        std::span(std::addressof(rhs), 1), 
        std::forward<OutputRange>(result)
    ); 
}

template<
    typename InputRange,
    typename OutputRange, 
    std::unsigned_integral Unsigned
>
requires TransformableToRange<OutputRange, InputRange, Unsigned>
constexpr auto add(
    InputRange&& lhs, 
    Unsigned rhs,
    OutputRange&& result
) -> Unsigned { 
    return add(
        std::forward<InputRange>(lhs), 
        std::span(std::addressof(rhs), 1), 
        std::forward<OutputRange>(result)
    ); 
}

}