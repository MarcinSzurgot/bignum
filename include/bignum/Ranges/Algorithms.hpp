#pragma once

#include <bignum/Concepts/Concepts.hpp>

#include <algorithm>
#include <ranges>

namespace bignum {

template<
    typename InputRange,
    typename Carry,
    typename OutputIterator, 
    typename BinaryOp
>
constexpr auto cascade(
    InputRange&& input,
    Carry initialCarry,
    OutputIterator output,
    BinaryOp&& op  
) -> std::pair<Carry, OutputIterator> {
    for(auto&& i : input) {
        std::tie(*output++, initialCarry) = op(i, std::move(initialCarry));
    }

    return std::make_pair(initialCarry, output);
}

template<
    std::ranges::input_range InputRange1,
    std::ranges::input_range InputRange2,
    typename OutputIterator,
    typename Carry,
    typename TernaryOperator
>
requires std::output_iterator<OutputIterator, std::iter_value_t<OutputIterator>>
constexpr auto cascade(
    InputRange1&& input1,
    InputRange2&& input2,
    Carry initialCarry,
    OutputIterator output,
    TernaryOperator&& op  
) -> std::pair<Carry, OutputIterator> {
    for (auto&& [i1, i2] : std::views::zip(input1, input2)) {
        std::tie(*output++, initialCarry) = op(i1, i2, std::move(initialCarry));
    }

    return std::make_pair(initialCarry, output);
}

}