#pragma once

#include <concepts>
#include <ranges>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>

namespace bignum {

template<
    std::ranges::input_range InputRange, 
    typename OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
requires TransformableToIter<OutputIterator, InputRange>
auto rshift(
    InputRange&& input,
    std::size_t shift,
    OutputIterator output
) -> Unsigned {
    shift &= Bits<Unsigned>::ShiftMask;

    auto first = begin(input);
    auto [outshifted, carry] = rshift(*first++, shift);

    carry = cascade(
        std::ranges::subrange(first, end(input)), carry, output, 
        [shift](auto&& next, auto carry) {
           const auto [lower, upper] = rshift(next, shift);
           return std::make_pair(carry | lower, upper);
        }
    ).first;

    *(output + (size(input) - 1)) = carry;

    return outshifted;
}

template<
    std::ranges::input_range InputRange, 
    typename OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
requires TransformableToIter<OutputIterator, InputRange>
auto lshift(
    InputRange&& input,
    std::size_t shift,
    OutputIterator result
) -> Unsigned {
    shift &= Bits<Unsigned>::ShiftMask;

    return cascade(input, Unsigned(), result,
        [shift] (auto&& next, auto carry) { 
           const auto [lower, upper] = lshift(next, shift); 
           return std::make_pair(carry | lower, upper);
        }
    ).first;
}

}