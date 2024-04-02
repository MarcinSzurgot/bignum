#pragma once

#include <algorithm>
#include <ranges>

namespace bignum {

template<
    std::ranges::input_range Range,
    typename Output,
    typename Carry,
    typename Functor,
    std::input_iterator... Inputs
> requires std::input_iterator<Output>
constexpr auto cascade(
    Range&& input1,
    Carry initialCarry,
    Output output,
    Functor&& func,
    Inputs ...inputs
) -> Carry {
    for (auto&& i1 : input1) {
        std::tie(*output++, initialCarry) = func(i1, *inputs++..., std::move(initialCarry));
    }

    return initialCarry;
}

}