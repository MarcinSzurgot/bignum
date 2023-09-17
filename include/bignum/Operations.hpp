#pragma once

#include <cstdint>
#include <span>
#include <vector>

auto sizeWithoutLeadingZeroes(
    std::span<const std::uint32_t> digits
) -> std::size_t;

// void subtract(
//     std::span<      std::uint32_t> bigger,
//     std::span<const std::uint32_t> smaller
// );

// auto topBit(
//     std::span<const std::uint32_t> digits
// ) -> std::size_t;

// auto divide(
//     std::span<const std::uint32_t> lhs,
//     std::span<const std::uint32_t> rhs
// ) -> std::pair<
//     std::vector<std::uint32_t>,
//     std::vector<std::uint32_t>
// >;