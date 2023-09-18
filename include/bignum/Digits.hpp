#pragma once

#include <cstdint>
#include <iostream>
#include <span>
#include <vector>

inline auto operator==(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs
) -> bool {
    if (size(lhs) != size(rhs)) {
        return false;
    }

    for (auto d = 0u; d < size(lhs); ++d) {
        if (lhs[d] != rhs[d]) {
            return false;
        }
    }

    return true;
}

inline auto operator<(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs
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

inline auto operator!=(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs
) -> bool { return !(lhs == rhs); }

inline auto operator>=(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs
) -> bool { return !(lhs < rhs); }

inline auto operator>(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs
) -> bool { return rhs < lhs; }

inline auto operator<=(
    std::span<const std::uint32_t> lhs,
    std::span<const std::uint32_t> rhs
) -> bool { return !(lhs > rhs); }

// auto operator<<=(
//     std::vector<std::uint32_t>& lhs,
//     std::uint32_t rhs
// ) -> std::vector<std::uint32_t>& {

// }

// auto operator>>=(
//     std::vector<std::uint32_t>& lhs,
//     std::uint32_t rhs
// ) -> std::vector<std::uint32_t>& {

// }

inline auto operator-=(
    std::span<      std::uint32_t> bigger,
    std::span<const std::uint32_t> smaller
) -> std::span<std::uint32_t> {
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

    return bigger;
}