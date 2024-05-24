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
    using InputTuple = std::tuple<Ins...>;

    Result result;
    Out out;
    InputTuple ins;
};

template<
    std::ranges::input_range Range,
    typename Output,
    typename Carry,
    typename Functor,
    typename Pred,
    std::input_iterator... Inputs
> requires std::input_iterator<Output>
constexpr auto cascadeUntil(
    Range&& input1,
    Carry initialCarry,
    Output output,
    Functor&& func,
    Pred&& pred,
    Inputs ...inputs
) -> CascadeResult<
    Carry, 
    Output, 
    std::ranges::iterator_t<Range>, 
    Inputs...
> {
    auto inputFirst = std::begin(input1);

    for(
        auto inputLast  = std::end(input1);
        pred(initialCarry) && inputFirst != inputLast;
        ++inputFirst,
        ++output
        ) {
        std::tie(*output, initialCarry) = func(
            *inputFirst, 
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
    return cascadeUntil(
        input1, 
        std::move(initialCarry), 
        output, 
        func, 
        [](auto){ return true; }, 
        inputs...
    );
}

template<
    std::ranges::input_range LongerOrEqual,
    std::ranges::input_range ShorterOrEqual,
    typename Output,
    typename Carry,
    typename Functor
> requires std::input_iterator<Output>
constexpr auto cascade(
    LongerOrEqual&& longerOrEqual,
    ShorterOrEqual&& shorterOrEqual,
    Carry initialCarry,
    Output output,
    Functor&& func
) -> CascadeResult<
    Carry, 
    Output, 
    std::ranges::iterator_t<LongerOrEqual>,
    std::ranges::iterator_t<ShorterOrEqual>
> {
    auto result1 = cascade(
        shorterOrEqual, initialCarry, output,
        [&func](auto rhs, auto lhs, auto carry) {
            return func(lhs, rhs, carry);
        },
        begin(longerOrEqual)
    );

    auto result2 = cascade(
        std::ranges::subrange(
            std::get<1>(result1.ins),
            end(longerOrEqual)
        ),
        result1.result,
        result1.out,
        func
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

}