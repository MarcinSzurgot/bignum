#include <bignum/Digits/Arithmetics.hpp>

#include <span>
#include <tuple>

#include <bignum/Access.hpp>
#include <bignum/Arrays/AdditiveOperations.hpp>

namespace bignum {

template<std::unsigned_integral U>
auto add(std::span<U> lhs, U rhs) -> U {
    return add(lhs, std::span(std::addressof(rhs), 1));
}

template<std::unsigned_integral U>
auto sub(std::span<U> lhs, U rhs) -> U {
    return sub(lhs, std::span(std::addressof(rhs), 1));
}

template<std::unsigned_integral U>
auto mul(std::span<U> lhs, U rhs) -> U {
    auto lower       = BigUnsigned::NativeDigit();
    auto higher      = BigUnsigned::NativeDigit();
    auto carry       = BigUnsigned::NativeDigit();
    auto firstCarry  = BigUnsigned::NativeDigit();
    auto secondCarry = BigUnsigned::NativeDigit();

    for (auto d = 0u; d < size(lhs); ++d) {
        const auto previousHigher = higher;

        std::tie(lower,     higher  ) = mul(lhs[d], rhs);
        std::tie(lhs[d], firstCarry ) = add(lower, previousHigher);
        std::tie(lhs[d], secondCarry) = add(lhs[d], carry);

        carry = firstCarry || secondCarry;
    }

    return higher + carry;
}

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