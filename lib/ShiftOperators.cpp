#include <bignum/ShiftOperators.hpp>

#include <bignum/Access.hpp>
#include <bignum/ArrayArithmetics/ArrayShift.hpp>
#include <bignum/Access.hpp>

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
    const auto wholeDigitsShift = rhs / BigUnsigned::NativeDigitBitSize;
    const auto bitShift = rhs % BigUnsigned::NativeDigitBitSize;
    const auto carry = (bool) (lhsAccess.digits().back() >> (BigUnsigned::NativeDigitBitSize - bitShift));

    if (wholeDigitsShift + carry + lhsAccess.size() > lhsAccess.capacity()) {
        auto shifted = std::vector<BigUnsigned::NativeDigit>(lhsAccess.size() + wholeDigitsShift + carry);

        bignum::leftShift(
            lhsAccess.digits(),
            rhs,
            std::span(shifted).subspan(wholeDigitsShift)
        );

        lhsAccess.swap(shifted);

    } else {
        lhsAccess.resize(lhsAccess.size() + wholeDigitsShift + carry);

        auto digits = lhsAccess.digits();

        std::fill(
            begin(digits), 
            begin(digits) + wholeDigitsShift, 0
        );

        bignum::leftShift(
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
    bignum::rightShift(lhs.access().digits(), rhs);
    return lhs;
}

}