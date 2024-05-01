#pragma once

#include <concepts>
#include <span>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Additive.hpp>

namespace bignum {

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    OutputIterator result
) -> CascadeResult<Unsigned, std::ranges::iterator_t<InputRange>, OutputIterator> {
    return cascade(lhs, Unsigned(), result, 
        [rhs, &result](auto next, auto carry){
            ++result;
            const auto [lower,     higher] = mul(next,    rhs);
            const auto [result, nextCarry] = add(lower, carry);
            return std::make_pair(result, nextCarry + higher);
    });
}

template<
    std::ranges::forward_range InputRange,
    std::forward_iterator InputIterator,
    std::forward_iterator OutputIterator,
    std::unsigned_integral Unsigned = std::iter_value_t<OutputIterator>
>
constexpr auto mul(
    InputRange&& lhs,
    Unsigned rhs,
    InputIterator adding,
    OutputIterator result
) -> std::pair<Unsigned, OutputIterator> {
    auto carry = cascade(lhs, Unsigned(), result, 
        [rhs, &result](auto&& next1, auto&& next2, auto&& carry){
            ++result;
            const auto [lower,     higher] = mul(next1,   rhs);
            const auto [result, nextCarry] = add(lower, next2, carry);
            return std::make_pair(result, nextCarry + higher);
    }, adding).result;

    return std::make_pair(carry, result);
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
    for (auto&& r : rhs) {
        auto [carry, continuation] = mul(lhs, r, output, output);
        ++output;

        for (; carry; ++continuation) {
            std::tie(*continuation, carry) = add(*continuation, carry);
        }
    }
}

template<
    std::ranges::input_range Minuend, 
    std::ranges::input_range Subtrahend, 
    std::unsigned_integral Unsigned,
    std::input_iterator Result,
    std::size_t MultiplierSize
> requires
    std::same_as<Unsigned, std::iter_value_t<Result>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Subtrahend>>
    && std::same_as<Unsigned, std::ranges::range_value_t<Minuend>>
constexpr auto mulSub(
    Minuend&& minuend,
    Subtrahend&& subtrahend,
    const std::array<Unsigned, MultiplierSize>& multiplier,
    Result result
) -> void {
    auto subCarry = Unsigned();

    auto finalMulCarry = Unsigned();
    auto multipliedSubtrahend = subtrahend | std::views::transform(
        [
            mulCarry = std::array<Unsigned, MultiplierSize + 1>(), 
            multiplier,
            &finalMulCarry
        ] (auto&& s) mutable {
            mul(
                multiplier,
                s,
                begin(mulCarry),
                begin(mulCarry)
            );

            const auto multiplied = mulCarry;
            std::copy(begin(mulCarry) + 1, end(mulCarry), begin(mulCarry));
            *(end(mulCarry) - 1) = Unsigned();

            finalMulCarry = mulCarry[0];

            return multiplied;
        }
    );

    auto minuendFirst = begin(minuend);

    for (auto&& s : multipliedSubtrahend) {
        std::tie(*result, subCarry) = sub(*minuendFirst, s[0], subCarry);
        ++result;
        ++minuendFirst;
    }

    if (finalMulCarry) {
        *result = sub(*(end(minuend) - 1), finalMulCarry, subCarry).first;
    }
}

}

