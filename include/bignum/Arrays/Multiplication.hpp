#pragma once

#include <concepts>
#include <span>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Additive.hpp>

namespace bignum {

// template<
//     std::input_iterator Result,
//     std::ranges::input_range    Range,
//     std::ranges::input_range ...Ranges
// > requires std::unsigned_integral<std::iter_value_t<Result>>
//     &&  std::same_as<std::iter_value_t<Result>, std::ranges::range_value_t<Range>>
//     && (std::same_as<std::iter_value_t<Result>, std::ranges::range_value_t<Ranges>> && ...)
// using MultiplicativeResult = CascadeResult<
//     std::iter_value_t<Result>,
//     Result,
//     std::ranges::iterator_t<Range>,
//     std::ranges::iterator_t<Ranges>...
// >;

template<
    std::ranges::input_range Augend,
    std::ranges::input_range Addend,
    std::input_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mulAdd(
    Augend&& augend,
    Addend&& addend,
    Unsigned multiplier,
    OutputIterator result
) {
    auto result1 = cascade(
        addend, 
        Unsigned(), 
        result, 
        [multiplier](auto a, auto m, auto carry) {
            const auto [product, productCarry] = mul(a, multiplier);
            const auto [sum,         sumCarry] = add(m, product, carry);
            return std::make_pair(sum, sumCarry + productCarry);
        }, std::begin(augend)
    );

    cascade(
        std::ranges::subrange(
            std::get<1>(result1.ins),
            end(augend)
        ),
        result1.result,
        result1.out,
        add<Unsigned>
    );
}

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    OutputIterator result
) -> CascadeResult<
    Unsigned, 
    OutputIterator,
    std::ranges::iterator_t<InputRange>
> {
    return cascade(lhs, Unsigned(), result,
        [rhs](auto left, auto carry){
            const auto [lower,     higher] = mul(left,    rhs);
            const auto [result, leftCarry] = add(lower, carry);
            return std::make_pair(result, leftCarry + higher);
    });
}

template<
    std::ranges::input_range Minuend, 
    std::ranges::input_range Subtrahend, 
    std::unsigned_integral Unsigned,
    std::input_iterator Result
> requires
    std::same_as<Unsigned, std::iter_value_t<Result>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Subtrahend>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Minuend>>
constexpr auto mulSub(
    Minuend&& minuend,
    Subtrahend&& subtrahend,
    Unsigned multiplier,
    Result result
) -> void {
    auto result1 = cascade(
        subtrahend, 
        Unsigned(), 
        result, 
        [multiplier](auto s, auto m, auto carry) {
            const auto [product, productCarry] = mul(s, multiplier);
            const auto [diff, diffCarry] = sub(m, product, carry);
            return std::make_pair(diff, diffCarry + productCarry);
        }, begin(minuend)
    );

    cascade(
        std::ranges::subrange(
            std::get<1>(result1.ins),
            end(minuend)
        ),
        result1.result,
        result1.out,
        sub<Unsigned>
    );
}

template<
    std::ranges::forward_range InputRange1, 
    std::ranges::forward_range InputRange2, 
    std::forward_iterator OutputIterator
>
constexpr auto mul(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputIterator output
) -> void {
    auto outputLast = std::next(output, size(lhs) + size(rhs));

    for (auto&& r : rhs) {
        mulAdd(
            std::ranges::subrange(
                output,
                outputLast
            ), 
            lhs,
            r,
            output
        );

        ++output;
    }
}

}

