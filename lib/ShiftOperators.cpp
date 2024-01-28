#include <bignum/ShiftOperators.hpp>

#include <bignum/Access.hpp>
#include <bignum/Arrays/Shifts.hpp>
#include <bignum/Access.hpp>
#include <bignum/Digits/Arithmetics.hpp>

#include <ranges>

namespace bignum {

auto operator<<(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    auto tmp = lhs;
    tmp <<= rhs;
    return tmp;
}

auto operator>>(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    auto tmp = lhs;
    tmp >>= rhs;
    return tmp;
}

auto operator<<=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
          auto lhsAccess = lhs.access();
    const auto [wholeDigitsShift, bitShift] = div(rhs, BigUnsigned::NativeDigitBitSize);

    if (wholeDigitsShift + lhsAccess.size() >= lhsAccess.capacity()) {
        auto shifted = std::vector<BigUnsigned::NativeDigit>(lhsAccess.size() + wholeDigitsShift + 1);

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

auto operator>>=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    auto digits = access.digits();
    rshift(digits, rhs, digits.begin());
    return lhs;
}

}