#pragma once 

#include <bignum/BigUnsigned.hpp>
#include <bignum/Arrays/Comparators.hpp>

namespace bignum {

inline auto operator==(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return lhs.digits() == rhs.digits(); }

inline auto operator!=(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return !(lhs == rhs); }

inline auto operator<(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return lhs.digits() < rhs.digits(); }

inline auto operator>=(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return !(lhs < rhs); }

inline auto operator>(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return rhs < lhs; }

inline auto operator<=(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return !(lhs > rhs); }

}