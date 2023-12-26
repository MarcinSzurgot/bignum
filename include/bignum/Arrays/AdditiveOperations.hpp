#pragma once

#include <concepts>
#include <span>  
#include <tuple>

#include <bignum/Digits/Arithmetics.hpp>

namespace bignum {

template<typename C, typename Arg>
concept BinaryOperator = requires(C c, Arg a1, Arg a2) {
    { c(a1, a2) } -> std::same_as<std::pair<Arg, Arg>>;
};

template<
    typename T, 
    typename U, 
    typename BinaryOp
>
requires std::unsigned_integral<T>
      && std::unsigned_integral<std::remove_const_t<U>> 
      && std::same_as<T, std::remove_const_t<U>>
      && BinaryOperator<BinaryOp, std::remove_const_t<T>>
auto transform(
    std::span<T> lhs,
    std::span<U> rhs,
    BinaryOp&& op  
) -> T {
    const auto common = std::min(size(lhs), size(rhs));

    auto carry       = T();
    auto firstCarry  = T();
    auto secondCarry = T();

    for (auto d = 0u; d < common; ++d) {
        std::tie(lhs[d], firstCarry)  = op(lhs[d], carry);
        std::tie(lhs[d], secondCarry) = op(lhs[d], rhs[d]);

        carry = firstCarry || secondCarry;
    }

    for (auto d = common; carry && d < size(lhs); ++d) {
        std::tie(lhs[d], carry) = op(lhs[d], carry);
    }

    return carry;
}

template<typename T, typename U>
requires std::unsigned_integral<T>
      && std::unsigned_integral<std::remove_const_t<U>> 
      && std::same_as<T, std::remove_const_t<U>>
auto subtract(
    std::span<T> lhs,
    std::span<U> rhs
) -> T { return transform(lhs, rhs, sub<T>); }

template<typename T, typename U>
requires std::unsigned_integral<T>
      && std::unsigned_integral<std::remove_const_t<U>> 
      && std::same_as<T, std::remove_const_t<U>>
auto add(
    std::span<T> lhs,
    std::span<U> rhs
) -> T { return transform(lhs, rhs, add<T>); }

}