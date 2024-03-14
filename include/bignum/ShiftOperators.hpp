#pragma once

#include <bignum/BigUnsigned.hpp>

namespace bignum {

template<
    std::unsigned_integral U, 
    std::unsigned_integral Shift
>
auto operator<<(
    const BigUnsigned<U>& lhs,
    Shift rhs
) -> BigUnsigned<U> {
    auto tmp = lhs;
    tmp <<= rhs;
    return tmp;
}

template<
    std::unsigned_integral U, 
    std::unsigned_integral Shift
>
auto operator>>(
    const BigUnsigned<U>& lhs,
    Shift rhs
) -> BigUnsigned<U> {
    auto tmp = lhs;
    tmp >>= rhs;
    return tmp;
}

template<
    std::unsigned_integral U, 
    std::unsigned_integral Shift
>
auto operator<<=(
    BigUnsigned<U>& lhs,
    Shift rhs
) -> BigUnsigned<U>& {
          auto lhsAccess = lhs.access();
    const auto [wholeDigitsShift, bitShift] = div<Shift>(rhs, Bits<U>::Size);

    if (wholeDigitsShift + lhsAccess.size() >= lhsAccess.capacity()) {
        auto shifted = std::vector<U>(lhsAccess.size() + wholeDigitsShift + 1);

        shifted.back() |= lshift(
            lhsAccess.digits(),
            rhs,
            begin(shifted) + wholeDigitsShift
        );

        lhsAccess.swap(shifted);

    } else {
        lhsAccess.resize(lhsAccess.size() + wholeDigitsShift + 1);

        auto digits = lhsAccess.digits();

        std::ranges::fill(digits.subspan(0, wholeDigitsShift), 0);

        digits.back() |= lshift(
            digits,
            rhs,
            begin(digits) + wholeDigitsShift
        );
    }

    return lhs;
}

template<
    std::unsigned_integral U, 
    std::unsigned_integral Shift
>
auto operator>>=(
    BigUnsigned<U>& lhs,
    Shift rhs
) -> BigUnsigned<U>& {
    auto access = lhs.access();
    auto digits = access.digits();
    rshift(digits, rhs, digits.begin());
    return lhs;
}

}