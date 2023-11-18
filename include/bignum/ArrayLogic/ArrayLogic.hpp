#pragma once

#include <concepts>
#include <span>

namespace bignum {

template<std::equality_comparable T>
auto isZero(
    std::span<T> array
) -> bool {
    return size(array) == 1ull && array[0] == T();
}

template<std::equality_comparable T>
auto operator==(
    std::span<const T> lhs,
    std::span<const T> rhs
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

template<std::equality_comparable T>
auto operator!=(
    std::span<const T> lhs,
    std::span<const T> rhs
) -> bool { return !(lhs == rhs); }

template<std::three_way_comparable T>
auto operator<(
    std::span<const T> lhs,
    std::span<const T> rhs
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

template<std::equality_comparable T>
auto operator>=(
    std::span<const T> lhs,
    std::span<const T> rhs
) -> bool { return !(lhs < rhs); }

template<std::equality_comparable T>
auto operator>(
    std::span<const T> lhs,
    std::span<const T> rhs
) -> bool { return rhs < lhs; }

template<std::equality_comparable T>
auto operator<=(
    std::span<const T> lhs,
    std::span<const T> rhs
) -> bool { return !(lhs > rhs); }

}