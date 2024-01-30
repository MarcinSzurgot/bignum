#pragma once

#include <algorithm>
#include <ranges>

namespace bignum {

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator OutputIterator,
    typename Carry,
    typename Functor,
    std::forward_iterator... InputIterators
>
constexpr auto cascade(
    InputRange&& input1,
    Carry initialCarry,
    OutputIterator output,
    Functor&& func,
    InputIterators ...inputs
) -> Carry {
    for (auto&& i1 : input1) {
        std::tie(*output++, initialCarry) = func(i1, *inputs++..., std::move(initialCarry));
    }

    return initialCarry;
}

}