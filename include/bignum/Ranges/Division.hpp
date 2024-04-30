#pragma once

#include <ranges>

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
    });
}

template<
    std::ranges::bidirectional_range Range,
    std::unsigned_integral Unsigned
> requires std::same_as<Unsigned, std::ranges::range_value_t<Range>>
constexpr auto approxDiv(
    Range&& dividend, // big-endian
    Unsigned divisor
) -> std::array<Unsigned, 2> {
    auto first = std::reverse_iterator(end(dividend));
    auto last = std::reverse_iterator(begin(dividend));

    if (first == last) {
        return std::array<Unsigned, 2>();
    }

    ++divisor;

    const auto topDvd = *first;
    if(++first == last) {
        return std::array<Unsigned, 2>{
            div(topDvd, divisor).first,
            Unsigned()
        };
    }
    
    return div(*first, topDvd, divisor).first;
}

}