#pragma once

#include <bitset>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Arrays/Shifts.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Additive.hpp>
#include <bignum/Ranges/Division.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<
    std::ranges::bidirectional_range Dividend,
    std::ranges::bidirectional_range Divisor,
    std::bidirectional_iterator Quotient,
    std::bidirectional_iterator Remainder,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<Dividend>
> requires 
    std::same_as<Unsigned, std::ranges::range_value_t<Divisor>>
    && std::same_as<Unsigned, std::iter_value_t<Quotient>>
    && std::same_as<Unsigned, std::iter_value_t<Remainder>>
// quotient should be at least size of the size(dividend) - size(divisor) + 1
// remainder should be at least size of the size(dividend)
constexpr auto div(
    Dividend&& dividend, // big-endian
    Divisor&& divisor, // big-endian
    Quotient quotient, // big-endian
    Remainder remainder // big-endian
) -> std::pair<Quotient, Remainder> {
    using namespace std::ranges;

    if (empty(divisor)) {
        throw std::runtime_error("Cannot divide by zero.");
    }

    auto remainderRange = subrange(
        remainder, 
        copy(dividend, remainder).out
    );

    auto quotientLast = std::next(
        quotient,
        size(dividend) < size(divisor)
        ? 0
        : size(dividend) - size(divisor) + 1
    );

    fill(subrange(quotient, quotientLast), Unsigned());

    for (
        auto approxDiv = bignum::approxDiv(remainderRange, divisor);
        approxDiv.first > 0;
        approxDiv      = bignum::approxDiv(remainderRange, divisor)
    ) {
        auto  quotientOffset = std::next(quotient, approxDiv.second);
        auto remainderOffset = std::next(remainder, approxDiv.second);

        auto quotientRange = subrange(
            quotientOffset,
            quotientLast
        );
        auto remainderRange2 = subrange(
            remainderOffset,
            remainderRange.end()
        );

        add(
            quotientRange,
            approxDiv.first,
            quotientRange.begin()
        );

        mulSub(
            remainderRange2,
            divisor,
            approxDiv.first,
            remainderRange2.begin()
        );

        remainderRange = subrange(remainder, trimm(remainderRange));
    }

    return {
        trimm(subrange(quotient, quotientLast)), 
        trimm(remainderRange)
    };
}

}