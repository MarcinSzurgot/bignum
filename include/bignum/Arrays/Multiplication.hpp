#pragma once

#include <concepts>
#include <span>

#include <bignum/Digits/Arithmetics.hpp>
#include <bignum/Utils.hpp>

namespace bignum {

template<
    typename T, 
    typename U, 
    typename K
> requires
    std::unsigned_integral<std::remove_const_t<T>>
    && std::unsigned_integral<std::remove_const_t<U>>
    && std::unsigned_integral<K>
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
    && std::same_as<std::remove_const_t<U>, K>
auto mul(
    std::span<T> lhs,
    std::span<U> rhs,
    std::span<K> result
) -> void {
    for (auto l = 0u; l < size(lhs); ++l) {
        auto carry       = K();
        auto firstCarry  = K();
        auto secondCarry = K();

        for (auto r = 0u; r < size(rhs); ++r) {
            const auto [lower, higher] = bignum::mul(lhs[l], rhs[r]);

            std::tie(result[l + r + 0], carry      ) = add(result[l + r + 0], lower);
            std::tie(result[l + r + 1], firstCarry ) = add(result[l + r + 1], higher);
            std::tie(result[l + r + 1], secondCarry) = add(result[l + r + 1], carry);

            carry = firstCarry || secondCarry;

            for (auto c = l + r + 2; carry && c < size(result); ++c) {
                std::tie(result[c], carry) = add(result[c], carry);
            }
        }
    }
}

}

