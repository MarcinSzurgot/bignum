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

auto operator+=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();

    if (add(access.digits(), std::span(std::addressof(rhs), 1))) {
        access.push_back(BigUnsigned::NativeDigit(1));
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

auto operator-=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    auto digits = access.digits();

    if (size(digits) == 1u && digits[0] < rhs) {
        digits[0] = rhs - digits[0];
    } else {
        subtract(digits, std::span(std::addressof(rhs), 1));
    }

    return lhs;
}

auto operator*=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    auto result = std::vector<BigUnsigned::NativeDigit>(
        size(lhs.digits()) + size(rhs.digits())
    );

    bignum::mul(
        lhs.digits(),
        rhs.digits(),
        std::span(result)
    );

    lhs.access().swap(result);
    return lhs;
}

auto operator*=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    auto digits = access.digits();

    auto lower       = BigUnsigned::NativeDigit();
    auto higher      = BigUnsigned::NativeDigit();
    auto carry       = BigUnsigned::NativeDigit();
    auto firstCarry  = BigUnsigned::NativeDigit();
    auto secondCarry = BigUnsigned::NativeDigit();

    for (auto d = 0u; d < size(digits); ++d) {
        const auto previousHigher = higher;

        std::tie(lower, higher) = bignum::mul(digits[d], rhs);
        std::tie(digits[d], firstCarry) = add(lower, previousHigher);
        std::tie(digits[d], secondCarry) = add(digits[d], carry);

        carry = firstCarry || secondCarry;
    }

    access.push_back(higher + carry);

    return lhs;
}

auto operator/=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {
    return lhs = divide(lhs, rhs).first;
}

auto operator/=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    
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

auto operator+(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
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

auto operator*(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
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