#pragma once

#include <ranges>

#include <bignum/Bits.hpp>
#include <bignum/Arrays/Comparators.hpp>
#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>

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
        distance
    };
}

template<
    std::ranges::bidirectional_range Dividend,
    std::unsigned_integral Unsigned
> requires std::same_as<Unsigned, std::ranges::range_value_t<Dividend>>
constexpr auto approxDiv(
    Dividend&& dividend, // big-endian
    Unsigned divisor
) -> std::array<Unsigned, 2> {
    auto first = std::reverse_iterator(end(dividend));
    auto last = std::reverse_iterator(begin(dividend));

    if (first == last) {
        return std::array<Unsigned, 2>();
    }

    const auto topDvd = *first;
    const auto [bumped, overflow] = add(divisor, Unsigned(1));
    
    if(overflow || ++first == last) {
        return std::array<Unsigned, 2>{
            div(topDvd, overflow ? overflow : bumped).first,
            Unsigned()
        };
    }
    
    return div(*first, topDvd, bumped).first;
}

}