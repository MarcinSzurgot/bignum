#pragma once

#include <concepts>
#include <span>  

namespace bignum {

template<typename T, typename U>
requires std::equality_comparable<                    T >
      && std::equality_comparable<std::remove_const_t<U>> 
      && std::same_as<
                                T, 
            std::remove_const_t<U>
        >
auto subtract(
    std::span<T> bigger,
    std::span<U> smaller
) -> bool {
    auto carry = false;
    for (auto d = 0u; d < size(smaller); ++d) {
        const auto old = bigger[d];
        bigger[d] -= smaller[d];
        bigger[d] -= carry;

        if (carry) {
            carry = bigger[d] >= old;
        } else {
            carry = bigger[d] >  old;
        }
    }

    for (auto d = size(smaller); d < size(bigger) && carry; ++d) {
        bigger[d] -= carry;
        carry = !(bigger[d] + 1);
    }

    return carry;
}

template<typename T, typename U>
requires std::equality_comparable<                    T >
      && std::equality_comparable<std::remove_const_t<U>> 
      && std::same_as<
                                T, 
            std::remove_const_t<U>
        >
auto add(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool {
    const auto common = std::min(size(lhs), size(rhs));

    auto carry = false;
    for (auto d = 0u; d < common; ++d) {
        const auto old = lhs[d];
        lhs[d] += rhs[d];
        lhs[d] += carry;

        if (carry) {
            carry = lhs[d] <= old;
        } else {
            carry = lhs[d] <  old;
        }
    }

    for (auto d = common; d < size(lhs) && carry; ++d) {
        lhs[d] += carry;
        carry = !lhs[d];
    }

    return carry;
}

}