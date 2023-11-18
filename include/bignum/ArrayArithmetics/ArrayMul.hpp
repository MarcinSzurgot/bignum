#pragma once

#include <concepts>
#include <span>

namespace bignum {

template<std::unsigned_integral U>
auto mul(
    std::span<const U> lhs,
    std::span<const U> rhs,
    std::span<      U> result
) -> void {
    constexpr auto digitBitSize = sizeof(U) * 8;

    for (auto l = 0u; l < size(lhs); ++l) {
        for (auto r = 0u; r < size(rhs); ++r) {
            const auto mul = (std::uint64_t) lhs[l] * rhs[r];
            const auto lower = BigUnsigned::digit_type(mul & ~BigUnsigned::digit_type());
            const auto higher = BigUnsigned::digit_type(mul >> digitBitSize);

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

