#pragma once 

#include <bignum/BigUnsigned.hpp>
#include <bignum/Arrays/Multiplication.hpp>
#include <bignum/Ranges/Additive.hpp>
#include <bignum/Ranges/Division.hpp>

#include <concepts>

namespace bignum {

template<std::unsigned_integral U>
auto divide(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> std::pair<BigUnsigned<U>, BigUnsigned<U>> {
    if (less(lhs, rhs)) {
        return {BigUnsigned(), lhs};
    }

    auto quotient  = std::vector<U>(size(lhs.digits()) - size(rhs.digits()) + 1); 
    auto remainder = std::vector<U>(size(lhs.digits()));

    const auto [quotLast, remLast] = div(
        lhs.digits(),
        rhs.digits(),
        begin(quotient),
        begin(remainder)
    );

    quotient.erase(quotLast, end(quotient));
    remainder.erase(remLast, end(remainder));

    return {
        BigUnsigned(std::move(quotient)), 
        BigUnsigned(std::move(remainder))
    };
}

template<std::unsigned_integral U>
auto operator+=(
          BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U>& {
    auto access = lhs.access();

    const auto lhsSize = access.size();
    const auto rhsSize = size(rhs.digits());

    if (lhsSize < rhsSize) {
        access.resize(rhsSize);
    } else if (lhsSize == rhsSize) {
        const auto lhsTop = access.digits().back();
        const auto rhsTop = rhs   .digits().back();

        if (add(lhsTop, rhsTop, U(1)).second) {
            access.reserve(lhsSize + 1);
        }
    }

    access.push_back(add(
        access.digits(),
        rhs.digits(),
        access.digits().begin()
    ).result);

    return lhs;
}

template<std::unsigned_integral U>
auto operator-=(
          BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U>& {

    if (less(lhs, rhs)) {
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

template<std::unsigned_integral U>
auto operator*=(
          BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U>& {
    auto result = std::vector<U>(
        size(lhs.digits()) + size(rhs.digits())
    );

    mul(lhs.digits(), rhs.digits(), begin(result));

    lhs.access().swap(result);
    return lhs;
}

template<std::unsigned_integral U>
auto operator/=(
          BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U>& {
    return lhs = divide(lhs, rhs).first;
}

template<std::unsigned_integral U>
auto operator%=(
          BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U>& {
    return lhs = divide(lhs, rhs).second;
}

template<std::unsigned_integral U>
auto operator+(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U> {
    auto result = lhs;
    result += rhs;
    return result;
}

template<std::unsigned_integral U>
auto operator-(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U> {
    auto result = lhs;
    result -= rhs;
    return result;
}

template<std::unsigned_integral U>
auto operator*(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U> {
    auto result = lhs;
    result *= rhs;
    return result;
}

template<std::unsigned_integral U>
auto operator/(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U> {
    auto result = lhs;
    result /= rhs;
    return result;
}

template<std::unsigned_integral U>
auto operator%(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> BigUnsigned<U> {
    auto result = lhs;
    result %= rhs;
    return result;
}

template<std::unsigned_integral U>
auto operator+=(
    BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U>& {
    auto access = lhs.access();
    auto digits = access.digits();
    access.push_back(add(digits, rhs, digits.begin()));
    return lhs;
}

template<std::unsigned_integral U>
auto operator-=(
    BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U>& {
    auto access = lhs.access();
    auto digits = access.digits();

    if (size(digits) == 1u && digits[0] < rhs) {
        digits[0] = rhs - digits[0];
    } else {
        sub(digits, rhs, digits.begin());
    }

    return lhs;
}

template<std::unsigned_integral U>
auto operator*=(
    BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U>& {
    auto access = lhs.access();
    auto digits = access.digits();

    access.push_back(mul(digits, rhs, digits.begin()).result);

    return lhs;
}

template<std::unsigned_integral U>
auto operator/=(
    BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U>& {
    
}

template<std::unsigned_integral U>
auto operator+(
    BigUnsigned<U>&& lhs,
    U rhs
) -> BigUnsigned<U> {
    lhs += rhs;
    return std::move(lhs);
}

template<std::unsigned_integral U>
auto operator+(
    const BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U> {
    return BigUnsigned(lhs) + rhs;
}

template<std::unsigned_integral U>
auto operator-(
    BigUnsigned<U>&& lhs,
    U rhs
) -> BigUnsigned<U> {
    lhs -= rhs;
    return std::move(lhs);
}

template<std::unsigned_integral U>
auto operator-(
    const BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U> {
    return BigUnsigned(lhs) - rhs;
}

template<std::unsigned_integral U>
auto operator*(
    BigUnsigned<U>&& lhs,
    U rhs
) -> BigUnsigned<U> {
    lhs *= rhs;
    return std::move(lhs);
}

template<std::unsigned_integral U>
auto operator*(
    const BigUnsigned<U>& lhs,
    U rhs
) -> BigUnsigned<U> {
    return BigUnsigned(lhs) * rhs;
}

}