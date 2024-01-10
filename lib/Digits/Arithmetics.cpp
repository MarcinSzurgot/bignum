#include <bignum/Digits/Arithmetics.hpp>

#include <span>
#include <tuple>

#include <bignum/Access.hpp>
#include <bignum/Arrays/AdditiveOperations.hpp>
#include <bignum/Arrays/Multiplication.hpp>

namespace bignum {

auto operator+=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    access.push_back(add(access.digits(), rhs));

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
        sub(digits, rhs);
    }

    return lhs;
}

auto operator*=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    auto access = lhs.access();
    auto digits = access.digits();

    access.push_back(mul(digits, rhs));

    return lhs;
}

auto operator/=(
    BigUnsigned& lhs,
    BigUnsigned::NativeDigit rhs
) -> BigUnsigned& {
    
}

}