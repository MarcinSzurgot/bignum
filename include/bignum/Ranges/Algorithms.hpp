#pragma once

#include <algorithm>
#include <ranges>
#include <tuple>

namespace bignum {

template<
    typename Result,
    std::input_iterator Out, 
    std::input_iterator ...Ins
>
struct CascadeResult {
    Result result;
    Out out;
    std::tuple<Ins...> ins;
};

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
) -> CascadeResult<
    Carry, 
    Output, 
    std::ranges::iterator_t<Range>, 
    Inputs...
> {
    using std::begin;
    using std::end;

    auto inputFirst = begin(input1);
    auto inputLast  = end(input1);

    while(inputFirst != inputLast) {
        std::tie(*output++, initialCarry) = func(
            *inputFirst++, 
            *inputs++..., 
            std::move(initialCarry)
        );
    }

    return CascadeResult {
        std::move(initialCarry),
        output,
        std::make_tuple(
            inputFirst,
            inputs...
        )
    };
}

}