#include <bignum/Digits/Arithmetics.hpp>

#include <span>
#include <tuple>

#include <bignum/Access.hpp>
#include <bignum/Arrays/AdditiveOperations.hpp>

namespace bignum {

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
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    
}

auto operator+(
    const BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned {
    auto result = lhs;
    result += rhs;
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

}