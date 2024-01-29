#pragma once 

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Concepts/Concepts.hpp>
#include <bignum/Ranges/Algorithms.hpp>

#include <concepts>
#include <ranges>
#include <tuple>

namespace bignum {

template<
    std::ranges::forward_range Input,
    std::unsigned_integral Unsigned,
    std::forward_iterator Output
> 
constexpr auto sub(
    Input&& lhs, 
    Unsigned rhs,
    Output output
) -> Unsigned { return cascade(lhs, rhs, output, sub<Unsigned>); }

template<
    std::ranges::forward_range Input,
    std::unsigned_integral Unsigned,
    std::forward_iterator Output
>
constexpr auto add(
    Input&& lhs, 
    Unsigned rhs,
    Output output
) -> Unsigned { return cascade(lhs, rhs, output, add<Unsigned>); }

template<
    std::ranges::forward_range InputRange1,
    std::ranges::forward_range InputRange2,
    std::forward_iterator OutputIterator,
    typename AdditiveOperator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto additive(
    InputRange1&& greater,
    InputRange2&& lesser,
    OutputIterator output,
    AdditiveOperator&& op
) -> Unsigned {
    auto firstGreater = begin(greater);
    const auto carry = cascade(
        lesser, Unsigned(), output, 
        [&](auto rhs, auto lhs, auto carry) {
            ++firstGreater;
            ++output;
            return op(lhs, rhs, carry);
        },
        firstGreater
    );

    return cascade(
        std::ranges::subrange(firstGreater, end(greater)), 
        carry, 
        output,
        op
    );
}

template<
    std::ranges::forward_range InputRange1,
    std::ranges::forward_range InputRange2,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto sub(
    InputRange1&& greater,
    InputRange2&& lesser,
    OutputIterator output
) -> Unsigned { 
    return additive(greater, lesser, output, [](auto&&... args){
        return sub(args...);
    });
}

template<
    std::ranges::forward_range InputRange1,
    std::ranges::forward_range InputRange2,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
> 
constexpr auto add(
    InputRange1&& greater,
    InputRange2&& lesser,
    OutputIterator output
) -> Unsigned { 
    return additive(greater, lesser, output, [](auto&&... args){
        return add(args...);
    });
}

}