#pragma once

#include <concepts>
#include <span>

namespace bignum {

template<
    typename MultiplicationResultType,
    typename T, 
    typename U, 
    typename K
> requires
    std::unsigned_integral<MultiplicationResultType>
    && std::unsigned_integral<std::remove_const_t<T>>
    && std::unsigned_integral<std::remove_const_t<U>>
    && std::unsigned_integral<K>
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
    && std::same_as<std::remove_const_t<U>, K>
    && (sizeof(MultiplicationResultType) == 2 * sizeof(K))
auto mul(
    std::span<T> lhs,
    std::span<U> rhs,
    std::span<K> result
) -> void {
    constexpr auto digitBitSize = sizeof(U) * 8;

    for (auto l = 0u; l < size(lhs); ++l) {
        for (auto r = 0u; r < size(rhs); ++r) {
            const auto mul = (MultiplicationResultType) lhs[l] * rhs[r];
            const auto lower = K(mul & ~K());
            const auto higher = K(mul >> digitBitSize);

            result[l + r] += lower;

            auto carry = result[l + r] < lower;
            result[l + r + 1] += carry;
            result[l + r + 1] += higher;
            
            if (carry) { 
                carry = result[l + r + 1] <= higher;
            } else {
                carry = result[l + r + 1] <  higher;
            }

            for (auto c = l + r + 2; c < size(result) && carry; ++c) {
                result[c] += carry;
                carry = !result[c];
            }
        }
    }
}

}

