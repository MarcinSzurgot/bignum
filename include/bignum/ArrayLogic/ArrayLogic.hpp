#pragma once

#include <concepts>
#include <span>

namespace bignum {

template<typename T>
requires std::equality_comparable<std::remove_const_t<T>> 
auto isZero(
    std::span<T> array
) -> bool {
    return size(array) == 1ull && array[0] == T();
}

template<typename T, typename U>
requires std::equality_comparable<std::remove_const_t<T>> 
    && std::equality_comparable<std::remove_const_t<U>> 
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
auto operator==(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool {
    if (size(lhs) != size(rhs)) {
        return false;
    }

    for (std::size_t i = 0; i < size(lhs); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, typename U>
requires std::equality_comparable<std::remove_const_t<T>> 
    && std::equality_comparable<std::remove_const_t<U>> 
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
auto operator!=(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool { return !(lhs == rhs); }

template<typename T, typename U>
requires std::three_way_comparable<std::remove_const_t<T>> 
    && std::three_way_comparable<std::remove_const_t<U>> 
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
auto operator<(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool {
    if (size(lhs) != size(rhs)) {
        return size(lhs) < size(rhs);
    }

    for (auto d = size(lhs); d > 0u; --d) {
        if (lhs[d - 1] != rhs[d - 1]) {
            return lhs[d - 1] < rhs[d - 1];
        }
    }

    return false;
}

template<typename T, typename U>
requires std::three_way_comparable<std::remove_const_t<T>> 
    && std::three_way_comparable<std::remove_const_t<U>> 
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
auto operator>=(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool { return !(lhs < rhs); }

template<typename T, typename U>
requires std::three_way_comparable<std::remove_const_t<T>> 
    && std::three_way_comparable<std::remove_const_t<U>> 
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
auto operator>(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool { return rhs < lhs; }

template<typename T, typename U>
requires std::three_way_comparable<std::remove_const_t<T>> 
    && std::three_way_comparable<std::remove_const_t<U>> 
    && std::same_as<std::remove_const_t<T>, std::remove_const_t<U>>
auto operator<=(
    std::span<T> lhs,
    std::span<U> rhs
) -> bool { return !(lhs > rhs); }

}