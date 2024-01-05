#include <bignum/ShiftOperators.hpp>

#include <bignum/Access.hpp>
#include <bignum/Arrays/Shifts.hpp>
#include <bignum/Access.hpp>
#include <bignum/Digits/Arithmetics.hpp>

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
            std::span(shifted).subspan(wholeDigitsShift)
        );

        lhsAccess.swap(shifted);

    } else {
        lhsAccess.resize(lhsAccess.size() + wholeDigitsShift + 1);

        auto digits = lhsAccess.digits();

        std::fill(
            begin(digits), 
            begin(digits) + wholeDigitsShift, 0
        );

        digits.back() |= lshift(
            digits,
            rhs,
            digits.subspan(wholeDigitsShift)
        );
    }

    return lhs;
}

auto operator>>=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    rshift(
        access.digits(), 
        rhs,
        access.digits()
    );
    return lhs;
}

}