#pragma once 

#include <algorithm>
#include <concepts>
#include <numeric>
#include <ranges>
#include <span>  
#include <tuple>

#include <bignum/Digits/Arithmetics.hpp>

namespace bignum {

template<typename Op, typename U> 
concept BinarySymmetricOperator = 
    std::unsigned_integral<U>
    && std::invocable<Op, U, U>
    && std::same_as<
        std::invoke_result_t<Op, U, U>, 
        std::pair<U, U>
    >;

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange, 
    typename BinaryOp
>
requires std::ranges::output_range<OutputRange, std::ranges::range_value_t<OutputRange>>
&& std::ranges::input_range<InputRange1>
&& std::ranges::input_range<InputRange2>
&& std::unsigned_integral<std::ranges::range_value_t<OutputRange>>
&& std::same_as<
    std::ranges::range_value_t<OutputRange>,
    std::ranges::range_value_t<InputRange1>
> && std::same_as<
    std::ranges::range_value_t<OutputRange>,
    std::ranges::range_value_t<InputRange2>
>
// > && BinarySymmetricOperator<
//     BinaryOp, 
//     std::ranges::range_value_t<OutputRange>
// >
constexpr auto scan(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result,
    BinaryOp&& op  
) -> std::ranges::range_value_t<OutputRange> {

    auto carry = std::ranges::range_value_t<OutputRange>();
    std::ranges::transform(
        std::views::zip(lhs, rhs),
        begin(lhs),
        [&](auto&& operands) {
            const auto [batch,  firstCarry] = op(operands.first, operands.second);
            const auto [final, secondCarry] = op(batch, carry);
            carry = firstCarry || secondCarry;
            return final;
        }
    );

    std::transform(
        begin(lhs) + std::min(size(lhs), size(rhs)),
        end(lhs),
        begin(lhs) + std::min(size(lhs), size(rhs)),
        [&](auto x) { 
            std::tie(x, carry) = op(x, carry);
            return x;
        }
    );

    return carry;
}

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange
>
requires std::ranges::output_range<OutputRange, std::ranges::range_value_t<OutputRange>>
&& std::ranges::input_range<InputRange1>
&& std::ranges::input_range<InputRange2>
&& std::unsigned_integral<std::ranges::range_value_t<OutputRange>>
&& std::same_as<
    std::ranges::range_value_t<OutputRange>,
    std::ranges::range_value_t<InputRange1>
> && std::same_as<
    std::ranges::range_value_t<OutputRange>,
    std::ranges::range_value_t<InputRange2>
>
constexpr auto sub(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> std::ranges::range_value_t<OutputRange> { 
    return scan(lhs, rhs, lhs, sub<std::ranges::range_value_t<OutputRange>>); 
}

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange
>
requires std::ranges::output_range<OutputRange, std::ranges::range_value_t<OutputRange>>
&& std::ranges::input_range<InputRange1>
&& std::ranges::input_range<InputRange2>
&& std::unsigned_integral<std::ranges::range_value_t<OutputRange>>
&& std::same_as<
    std::ranges::range_value_t<OutputRange>,
    std::ranges::range_value_t<InputRange1>
> && std::same_as<
    std::ranges::range_value_t<OutputRange>,
    std::ranges::range_value_t<InputRange2>
>
constexpr auto add(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result
) -> std::ranges::range_value_t<OutputRange> { 
    return scan(lhs, rhs, result, add<std::ranges::range_value_t<OutputRange>>); 
}

template<
    typename OutputRange, 
    typename Unsigned
>
requires std::ranges::output_range<OutputRange, std::ranges::range_value_t<OutputRange>>
&& std::unsigned_integral<std::ranges::range_value_t<OutputRange>>
&& std::unsigned_integral<Unsigned> 
&& std::same_as<
    std::ranges::range_value_t<OutputRange>,
    Unsigned
>
constexpr auto sub(
    OutputRange&& lhs, 
    Unsigned rhs
) -> Unsigned { return sub(lhs, std::span(std::addressof(rhs), 1), lhs); }

template<
    typename OutputRange, 
    typename Unsigned
>
requires std::ranges::output_range<OutputRange, std::ranges::range_value_t<OutputRange>>
&& std::unsigned_integral<std::ranges::range_value_t<OutputRange>>
&& std::unsigned_integral<Unsigned> 
&& std::same_as<
    std::ranges::range_value_t<OutputRange>,
    Unsigned
>
constexpr auto add(
    OutputRange&& lhs, 
    Unsigned rhs
) -> Unsigned { return add(lhs, std::span(std::addressof(rhs), 1), lhs); }

}