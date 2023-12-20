#include <bignum/ArithmeticOperators.hpp>

#include <bignum/BigUnsigned.hpp>
#include <bignum/LogicOperators.hpp>
#include <bignum/ShiftOperators.hpp>

namespace bignum {

auto divide(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> std::pair<BigUnsigned, BigUnsigned> {
    if (lhs < rhs) {
        return {BigUnsigned(), lhs};
    }

    const auto lhsDigits = lhs.digits();
    const auto rhsDigits = rhs.digits();

    const auto rhsTopBit = topBit(rhsDigits);
    const auto lhsTopBit = topBit(lhsDigits);

    auto bitDiff = lhsTopBit - rhsTopBit;
    auto divider = rhs << bitDiff;
    auto quotient  = std::vector<BigUnsigned::NativeDigit>(bitDiff / BigUnsigned::NativeDigitBitSize + 1); 
    auto remainder = std::vector<BigUnsigned::NativeDigit>(size(lhsDigits));

    const auto [quotSize, remSize] = bignum::divide(
        lhsDigits,
        rhsDigits,
        std::span(quotient),
        std::span(remainder)
    );

    quotient.resize(quotSize);
    remainder.resize(remSize);

    return {
        BigUnsigned(std::move(quotient)), 
        BigUnsigned(std::move(remainder))
    };
}

auto operator+=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {

    lhs.operate([&](auto& digits) {

        const auto lhsSize = size(    digits  );
        const auto rhsSize = size(rhs.digits());

        if (lhsSize < rhsSize) {
            digits.resize(rhsSize);
        } else if (lhsSize == rhsSize) {
            const auto lhsTop = digits      .back();
            const auto rhsTop = rhs.digits().back();

            if (lhsTop + rhsTop + 1 < lhsTop) {
                digits.reserve(lhsSize + 1);
            }
        }

        if (bignum::add(
            std::span(    digits  ),
            std::span(rhs.digits())
        )) {
            digits.push_back(1);
        }
    });

    return lhs;
}

auto operator-=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {

    lhs.operate([&](auto& digits) {
        if (lhs < rhs) {
            auto tmp = rhs;
            bignum::subtract(
                std::span(tmp.digits()),
                std::span(digits)
            );
            tmp.swap(digits);

        } else {
            bignum::subtract(
                std::span(digits),
                std::span(rhs.digits())
            );
        }
    });

    return lhs;
}

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    using MultiplicationType = std::uint32_t;
    using MultiplicationContainingType = std::uint64_t;

    static_assert(sizeof(MultiplicationType) <= sizeof(BigUnsigned::NativeDigit));
    static_assert(sizeof(MultiplicationContainingType) == sizeof(MultiplicationType) * 2);

    auto result = std::vector<BigUnsigned::NativeDigit>(
        size(lhs.digits()) + size(rhs.digits())
    );

    auto resultSpan = std::span(
        reinterpret_cast<MultiplicationContainingType*>(begin(result).base()),
        reinterpret_cast<MultiplicationContainingType*>(end(result).base())
    );

    bignum::mul<MultiplicationContainingType>(
        lhs.digits<const MultiplicationContainingType>(),
        rhs.digits<const MultiplicationContainingType>(),
        resultSpan
    );

    lhs.swap(result);
    return lhs;
}

auto operator/=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = divide(lhs, rhs).first;
}

auto operator%=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = divide(lhs, rhs).second;
}

auto operator+(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result += rhs;
    return result;
}

auto operator-(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result -= rhs;
    return result;
}

auto operator*(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result *= rhs;
    return result;
}

auto operator/(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result /= rhs;
    return result;
}

auto operator%(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned {
    auto result = lhs;
    result %= rhs;
    return result;
}

}