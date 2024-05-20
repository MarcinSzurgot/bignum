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
    std::ranges::forward_range Input,
    std::unsigned_integral Unsigned,
    std::forward_iterator Output
> 
constexpr auto sub(
    Input&& lhs, 
    Unsigned rhs,
    Output output
) -> AdditiveResult<Output, Input> { 
    return cascade(lhs, rhs, output, sub<Unsigned>); 
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
    return cascade(lhs, rhs, output, add<Unsigned>);
}

template<
    std::ranges::forward_range InputRange1,
    std::ranges::forward_range InputRange2,
    std::forward_iterator OutputIterator,
    typename AdditiveOperator
>
constexpr auto additive(
    InputRange1&& greater,
    InputRange2&& lesser,
    OutputIterator output,
    AdditiveOperator&& op
) -> AdditiveResult<
    OutputIterator,
    InputRange1, 
    InputRange2
> {
    auto result1 = cascade(
        lesser, std::iter_value_t<OutputIterator>(), output, 
        [&op](auto rhs, auto lhs, auto carry) {
            return op(lhs, rhs, carry);
        },
        begin(greater)
    );

    auto result2 = cascade(
        std::ranges::subrange(
            std::get<1>(result1.ins),
            end(greater)
        ),
        result1.result,
        result1.out,
        op
    );

    return {
        result2.result,
        result2.out,
        std::make_tuple(
            std::get<0>(result2.ins),
            std::get<0>(result1.ins)
        )
    };
}

template<
    std::ranges::forward_range InputRange1,
    std::ranges::forward_range InputRange2,
    std::forward_iterator OutputIterator
>
constexpr auto sub(
    InputRange1&& greater,
    InputRange2&& lesser,
    OutputIterator output
) -> AdditiveResult<
    OutputIterator,
    InputRange1,
    InputRange2
> { 
    return additive(greater, lesser, output, [](auto... args){
        return sub(args...);
    });
}

template<
    std::ranges::input_range InputRange1,
    std::ranges::input_range InputRange2,
    std::input_iterator OutputIterator
> 
constexpr auto add(
    InputRange1&& greater,
    InputRange2&& lesser,
    OutputIterator output
) -> AdditiveResult<
    OutputIterator,
    InputRange1,
    InputRange2
> { 
    return additive(greater, lesser, output, [](auto... args){
        return add(args...);
    });
}

}