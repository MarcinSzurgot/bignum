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
) -> Unsigned { return transformWithCarry(lhs, rhs, result, sub<Unsigned>); }

template<
    std::ranges::input_range InputRange1,
    std::ranges::input_range InputRange2,
    typename OutputRange,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<std::remove_reference_t<OutputRange>>
> 
requires TransformableToRange<
    std::remove_reference_t<OutputRange>, 
    std::remove_reference_t<InputRange1>, 
    std::remove_reference_t<InputRange2>
>
constexpr auto add(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> Unsigned { return transformWithCarry(lhs, rhs, result, add<Unsigned>); }

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
) -> Unsigned { return transformWithCarry(lhs, rhs, begin(result), sub<Unsigned>); }

template<
    typename InputRange,
    typename OutputRange, 
    std::unsigned_integral Unsigned = std::ranges::range_value_t<std::remove_reference_t<OutputRange>>
>
requires TransformableToRange<
    std::remove_reference_t<OutputRange>, 
    std::remove_reference_t<InputRange>, 
    Unsigned
>
constexpr auto add(
    InputRange&& lhs, 
    Unsigned rhs,
    OutputRange&& result
) -> Unsigned { return transformWithCarry(lhs, rhs, begin(result), add<Unsigned>); }

}