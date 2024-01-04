#pragma once

#include <concepts>
#include <span>

namespace bignum {

template<typename T>
requires std::equality_comparable<std::remove_const_t<T>>
auto operator!(
    std::span<T> array
) -> bool {
    return size(array) == typename std::span<T>::size_type(1) && array.front() == T();
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

    for (auto i = typename std::span<U>::size_type(); i < size(lhs); ++i) {
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

    for (auto d = size(lhs); d > typename std::span<T>::size_type(); --d) {
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