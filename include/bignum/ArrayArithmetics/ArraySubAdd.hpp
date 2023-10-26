#pragma once

#include <concepts>
#include <span>

namespace bignum {

template<std::unsigned_integral U>
auto subtract(
    std::span<      U> bigger,
    std::span<const U> smaller
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

template<std::unsigned_integral U>
auto add(
    std::span<      U> lhs,
    std::span<const U> rhs
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