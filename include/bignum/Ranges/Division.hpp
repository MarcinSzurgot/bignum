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

}