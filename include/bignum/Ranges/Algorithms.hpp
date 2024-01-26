#pragma once

#include <bignum/Concepts/Concepts.hpp>

#include <algorithm>
#include <ranges>

namespace bignum {

template<
    typename InputRange,
    typename OutputIter, 
    typename BinaryOp,
    std::unsigned_integral Unsigned = std::iter_value_t<std::remove_reference_t<OutputIter>>
>
requires TransformableToIter<
    std::remove_reference_t<OutputIter>, 
    std::remove_reference_t<InputRange>
>
constexpr auto transformWithCarry(
    InputRange&& input,
    Unsigned initialCarry,
    OutputIter&& out,
    BinaryOp&& op  
) -> Unsigned {
    for(auto&& i : input) {
        std::tie(*out++, initialCarry) = op(i, initialCarry);
    }
    return initialCarry;
}

template<
    typename InputRange1,
    typename InputRange2,
    typename OutputRange, 
    typename BinaryOp,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<std::remove_reference_t<OutputRange>>
>
requires TransformableToRange<
    std::remove_reference_t<OutputRange>, 
    std::remove_reference_t<InputRange1>, 
    std::remove_reference_t<InputRange2>
>
constexpr auto transformWithCarry(
    InputRange1&& lhs,
    InputRange2&& rhs,
    OutputRange&& result,
    BinaryOp&& op  
) -> Unsigned {

    auto carry = Unsigned();

    auto firstIter1 = begin(lhs);
    auto lastIter1  = end(lhs);
    auto firstIter2 = begin(rhs);
    auto lastIter2  = end(rhs);
    auto outIter = begin(result);

    for (; firstIter1 != lastIter1 && firstIter2 != lastIter2;
            ++firstIter1, ++firstIter2, ++outIter) {
        const auto [batch,  firstCarry] = op(*firstIter1, *firstIter2);
        const auto [final, secondCarry] = op(batch, carry);
        carry = firstCarry || secondCarry;
        *outIter = final;
    }

    return transformWithCarry(
        std::ranges::subrange(firstIter1, lastIter1),
        carry, 
        outIter,
        std::forward<BinaryOp>(op)
    );
}

}