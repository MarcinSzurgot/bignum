#include <bignum/ShiftOperators.hpp>

#include <bignum/ArrayArithmetics/ArrayShift.hpp>

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

    lhs.operate([&rhs](std::vector<BigUnsigned::NativeDigit>& digits){ 

        const auto wholeDigitsShift = rhs / BigUnsigned::NativeDigitBitSize;
        const auto bitShift = rhs % BigUnsigned::NativeDigitBitSize;
        const auto carry = (bool) (digits.back() >> (BigUnsigned::NativeDigitBitSize - bitShift));

        if (wholeDigitsShift + carry + size(digits) > digits.capacity()) {
            auto shifted = std::vector<BigUnsigned::NativeDigit>(size(digits) + wholeDigitsShift + carry);

            bignum::leftShift(
                std::span(digits),
                rhs,
                std::span(shifted).subspan(wholeDigitsShift)
            );

            std::swap(digits, shifted);

        } else {
            digits.resize(size(digits) + wholeDigitsShift + carry);

            std::fill(begin(digits), begin(digits) + wholeDigitsShift, 0);

            bignum::leftShift(
                std::span(digits),
                rhs,
                std::span(digits).subspan(wholeDigitsShift)
            );
        }
    });

    return lhs;
}

auto operator>>=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    const auto newSize = bignum::rightShift(lhs.digits(), rhs);

    lhs.operate([newSize](auto& digits) {
        digits.resize(size(digits) - newSize);
    });

    return lhs;
}

}