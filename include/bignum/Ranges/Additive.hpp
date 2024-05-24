#pragma once 

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>

#include <concepts>
#include <ranges>
#include <tuple>

namespace bignum {

template<
    std::input_iterator Output,
    std::ranges::input_range    Range,
    std::ranges::input_range ...Ranges
> requires std::unsigned_integral<std::iter_value_t<Output>>
    &&  std::same_as<std::iter_value_t<Output>, std::ranges::range_value_t<Range>>
    && (std::same_as<std::iter_value_t<Output>, std::ranges::range_value_t<Ranges>> && ...)
using AdditiveResult = CascadeResult<
    std::iter_value_t<Output>,
    Output,
    std::ranges::iterator_t<Range>,
    std::ranges::iterator_t<Ranges>...
>;

template<
    std::ranges::input_range Input,
    std::unsigned_integral Unsigned,
    std::input_iterator Output
> 
constexpr auto sub(
    Input&& lhs, 
    Unsigned rhs,
    Output output
) -> AdditiveResult<Output, Input> { 
    return cascadeUntil(
        lhs,
        rhs,
        output,
        sub<Unsigned>,
        [](auto value) { return value != Unsigned(); }
    );
}

template<
    std::ranges::forward_range Input,
    std::unsigned_integral Unsigned,
    std::forward_iterator Output
>
constexpr auto add(
    Input&& lhs, 
    Unsigned rhs,
    Output output
) -> AdditiveResult<Output, Input> {
        return cascadeUntil(
        lhs,
        rhs,
        output,
        add<Unsigned>,
        [](auto value) { return value != Unsigned(); }
    );
}

template<
    std::ranges::input_range LongerOrEqual,
    std::ranges::input_range ShorterOrEqual,
    std::input_iterator Output,
    std::unsigned_integral Unsigned = std::iter_value_t<Output>
> requires 
    std::same_as<Unsigned, std::ranges::range_value_t<LongerOrEqual>>
    && std::same_as<Unsigned, std::ranges::range_value_t<ShorterOrEqual>>
constexpr auto sub(
    LongerOrEqual&& longerOrEqual,
    ShorterOrEqual&& shorterOrEqual,
    Output output
) -> AdditiveResult<
    Output,
    LongerOrEqual,
    ShorterOrEqual
> { 
    return cascade(
        longerOrEqual, 
        shorterOrEqual,
        Unsigned(),
        output,
        [](auto... values) { return sub(values...); }
    );
}

template<
    std::ranges::input_range LongerOrEqual,
    std::ranges::input_range ShorterOrEqual,
    std::input_iterator Output,
    std::unsigned_integral Unsigned = std::iter_value_t<Output>
> requires 
    std::same_as<Unsigned, std::ranges::range_value_t<LongerOrEqual>>
    && std::same_as<Unsigned, std::ranges::range_value_t<ShorterOrEqual>>
constexpr auto add(
    LongerOrEqual&& longerOrEqual,
    ShorterOrEqual&& shorterOrEqual,
    Output output
) -> AdditiveResult<
    Output,
    LongerOrEqual,
    ShorterOrEqual
> { 
    return cascade(
        longerOrEqual, 
        shorterOrEqual,
        Unsigned(),
        output,
        [](auto... values) { return add(values...); }
    );
}

}