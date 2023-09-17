#include "bignum/Operations.hpp"

#include <algorithm>
#include <iostream>

namespace {

auto leadingZeroes(
    std::span<const std::uint32_t> digits
) -> std::size_t {
    return std::find_if(
        rbegin(digits),
        rend(digits),
        [](auto digit) { return digit > 0; }
    ) - rbegin(digits);
}

}

auto sizeWithoutLeadingZeroes(
    std::span<const std::uint32_t> digits
) -> std::size_t {
    return std::max(size(digits) - leadingZeroes(digits), std::size_t(1));
}

// void subtract(
//     std::span<      std::uint32_t> bigger,
//     std::span<const std::uint32_t> smaller
// ) {
//     auto carry = false;
//     for (auto d = 0u; d < size(smaller); ++d) {
//         const auto old = bigger[d];
//         bigger[d] -= smaller[d];
//         bigger[d] -= carry;

//         if (carry) {
//             carry = bigger[d] >= old;
//         } else {
//             carry = bigger[d] >  old;
//         }
//     }

//     for (auto d = size(smaller); d < size(bigger) && carry; ++d) {
//         bigger[d] -= carry;
//         carry = !(bigger[d] + 1);
//     }
// }

auto topBit(
    std::span<const std::uint32_t> digits
) -> std::size_t {
    if (empty(digits) || (size(digits) == 1u && digits.front() == 0u)) { 
        return 0u;
    }

    const auto topDigit = digits.back();

    auto bit = 31u;
    for (; bit > 0u && !((1 << bit) & topDigit); --bit) {

    }

    return bit + (size(digits) - 1) * 32;
}

// auto divide(
//     std::span<const std::uint32_t> lhs,
//     std::span<const std::uint32_t> rhs
// ) -> std::pair<
//     std::vector<std::uint32_t>,
//     std::vector<std::uint32_t>
// >;