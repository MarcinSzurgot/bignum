#pragma once

#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Arrays/Shifts.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>
#include <bignum/Ranges/Comparators.hpp>
#include <bignum/Utils.hpp>

#include <ranges>

namespace bignum {

template<
    std::ranges::input_range Range,
    std::unsigned_integral U,
    typename Output
> requires std::output_iterator<Output, U>
constexpr auto div(
    Range&& dividend,
    U divisor,
    Output quotient
) -> U {
    return cascade(dividend, U(), quotient, [divisor](auto&& d, auto&& carry) {
        const auto [quotient, remainder] = div(d, carry, divisor);
        return std::make_pair(quotient[0], remainder);
    }).result;
}

template<
    std::ranges::bidirectional_range Dividend,
    std::ranges::bidirectional_range Divisor,
    std::unsigned_integral Unsigned = std::ranges::range_value_t<Dividend>,
    typename Distance = std::ranges::range_difference_t<Divisor>
> requires std::same_as<Unsigned, std::ranges::range_value_t<Divisor>>
    && std::ranges::sized_range<Dividend>
    && std::ranges::sized_range<Divisor>
constexpr auto approxDiv(
    Dividend&& dividend, // big-endian
    Divisor&& divisor // big-endian
) -> std::pair<Unsigned, Distance> {
    if (less(dividend, divisor)) {
        return {Unsigned(), Distance()};
    }

    auto dividendRevFirst =  std::reverse_iterator(end(dividend));
    const auto divisorTop = *std::reverse_iterator(end(divisor));

    // if dividend is less than divisor and has size == 1 then divisor also is 1
    if (size(dividend) == 1) {
        return {Unsigned(*dividendRevFirst / divisorTop), Distance()};
    }

    const auto distance = Distance(size(dividend) - size(divisor));

    // If sizes are equal and biggest digits are equal then it will fit only once
    if (distance == Distance() && *dividendRevFirst == divisorTop) {
        return {Unsigned(1), distance};
    }

    // divisor size is 1 but dividend size is greater
    if (size(divisor) == 1) {
        if (*dividendRevFirst >= divisorTop) {
            return {Unsigned(*dividendRevFirst / divisorTop), distance};
        }

        return {
            div(
                *std::next(dividendRevFirst),
                *dividendRevFirst,
                divisorTop
            ).first[0],
            Distance(distance - 1)
        };
    }

    if (*dividendRevFirst > divisorTop) {
        return {
            Unsigned(*dividendRevFirst / Unsigned(divisorTop + 1)),
            distance
        };
    }

    if (divisorTop == Unsigned(~Unsigned())) {
        return {*dividendRevFirst, Distance(distance - 1)};
    }

    return {
        div(
            *std::next(dividendRevFirst),
            *dividendRevFirst,
            Unsigned(divisorTop + 1)
        ).first[0], 
        Distance(distance - 1)
    };
}

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