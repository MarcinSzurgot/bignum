#pragma once

#include <ranges>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Ranges/Algorithms.hpp>

namespace bignum {

template<
    std::ranges::forward_range InputRange,
    std::unsigned_integral U,
    std::forward_iterator OutputIterator
>
constexpr auto div(
    InputRange&& dividend,
    U divisor,
    OutputIterator quotient
) -> U {
    return cascade(dividend, U(), quotient, [divisor](auto&& d, auto&& carry) {
        const auto [quotient, remainder] = div(d, carry, divisor);
        return std::make_pair(quotient[0], remainder);
    });
}

}