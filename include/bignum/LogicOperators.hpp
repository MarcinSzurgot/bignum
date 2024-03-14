#pragma once 

#include <bignum/BigUnsigned.hpp>
#include <bignum/Arrays/Comparators.hpp>

namespace bignum {

template<std::unsigned_integral U>
auto operator==(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return lhs.digits() == rhs.digits(); }

template<std::unsigned_integral U>
auto operator!=(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return !(lhs == rhs); }

template<std::unsigned_integral U>
auto operator<(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return lhs.digits() < rhs.digits(); }

template<std::unsigned_integral U>
auto operator>=(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return !(lhs < rhs); }

template<std::unsigned_integral U>
auto operator>(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return rhs < lhs; }

template<std::unsigned_integral U>
auto operator<=(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return !(lhs > rhs); }

}