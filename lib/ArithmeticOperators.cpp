#include <bignum/ArithmeticOperators.hpp>

#include <bignum/Access.hpp>
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
    auto lhsAccess = lhs.access();

    const auto lhsSize = lhsAccess.size();
    const auto rhsSize = size(rhs.digits());

    if (lhsSize < rhsSize) {
        lhsAccess.resize(rhsSize);
    } else if (lhsSize == rhsSize) {
        const auto lhsTop = lhsAccess.digits().back();
        const auto rhsTop = rhs      .digits().back();

        if (lhsTop + rhsTop + 1 < lhsTop) {
            lhsAccess.reserve(lhsSize + 1);
        }
    }

    if (bignum::add(
        lhsAccess.digits(),
        rhs.digits()
    )) {
        lhsAccess.push_back(1);
    }

    return lhs;
}

auto operator-=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {

    if (lhs < rhs) {
        auto tmp = rhs;
        bignum::subtract(
            tmp.access().digits(),
            lhs.digits()
        );
        std::swap(lhs, tmp);

    } else {
        bignum::subtract(
            lhs.access().digits(),
            rhs.digits()
        );
    }

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
        reinterpret_cast<MultiplicationType*>(begin(result).base()),
        reinterpret_cast<MultiplicationType*>(end(result).base())
    );

    bignum::mul<MultiplicationContainingType>(
        lhs.digits<const MultiplicationType>(),
        rhs.digits<const MultiplicationType>(),
        resultSpan
    );

    lhs.access().swap(result);
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