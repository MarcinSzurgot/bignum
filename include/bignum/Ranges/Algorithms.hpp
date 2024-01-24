#pragma once

#include <bignum/Concepts/Concepts.hpp>

#include <algorithm>
#include <ranges>

namespace bignum {

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange, 
    typename BinaryOp,
    typename ReturnType = std::ranges::range_value_t<OutputRange>
>
requires TransformableToRange<OutputRange, InputRange1, InputRange2>
constexpr auto transformWithCarry(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result,
    BinaryOp&& op  
) -> ReturnType {

    auto carry = ReturnType();

    auto outIter = begin(result);
    for (const auto& [l, r] : std::views::zip(
        std::forward<InputRange1>(lhs), 
        std::forward<InputRange2>(rhs)
    )) {
        const auto [batch,  firstCarry] = op(l, r);
        const auto [final, secondCarry] = op(batch, carry);
        carry = firstCarry || secondCarry;
        *outIter++ = final;
    }

    for (auto lastOut = end(result); outIter != lastOut; ++outIter) {
        std::tie(*outIter, carry) = op(*outIter, carry);
    }

    return carry;
}

}