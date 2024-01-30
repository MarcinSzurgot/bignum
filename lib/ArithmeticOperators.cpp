#include <bignum/ArithmeticOperators.hpp>

#include <bignum/Access.hpp>
#include <bignum/BigUnsigned.hpp>
#include <bignum/Digits/Arithmetics.hpp>
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

    auto quotient  = std::vector<BigUnsigned::NativeDigit>(size(lhs.digits()) - size(rhs.digits()) + 1); 
    auto remainder = std::vector<BigUnsigned::NativeDigit>(size(lhs.digits()));

    const auto [quotSize, remSize] = div(
        lhs.digits(),
        rhs.digits(),
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
    auto access = lhs.access();

    const auto lhsSize = access.size();
    const auto rhsSize = size(rhs.digits());

    if (lhsSize < rhsSize) {
        access.resize(rhsSize);
    } else if (lhsSize == rhsSize) {
        const auto lhsTop = access.digits().back();
        const auto rhsTop = rhs   .digits().back();

        if (add(lhsTop, rhsTop, BigUnsigned::NativeDigit(1)).second) {
            access.reserve(lhsSize + 1);
        }
    }

    access.push_back(add(
        access.digits(),
        rhs.digits(),
        access.digits().begin()
    ));

    return lhs;
}

auto operator-=(
          BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> BigUnsigned& {

    if (lhs < rhs) {
        auto tmp = rhs;
        sub(
            tmp.access().digits(),
            lhs.digits(),
            tmp.access().digits().begin()
        );
        std::swap(lhs, tmp);

    } else {
        sub(
            lhs.access().digits(),
            rhs.digits(),
            lhs.access().digits().begin()
        );
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

    mul(lhs.digits(), rhs.digits(), begin(result));

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

auto operator+=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    auto digits = access.digits();
    access.push_back(add(digits, rhs, digits.begin()));
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
        sub(digits, rhs, digits.begin());
    }

    return lhs;
}

auto operator*=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    auto digits = access.digits();

    access.push_back(mul(digits, rhs, digits.begin()));

    return lhs;
}

auto operator/=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    
}

}