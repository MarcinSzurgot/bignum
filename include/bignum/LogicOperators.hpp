#pragma once 

#include <bignum/BigUnsigned.hpp>
#include <bignum/Arrays/Comparators.hpp>

namespace bignum {

template<std::unsigned_integral U>
auto equal(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return equal(lhs.digits(), rhs.digits()); }

template<std::unsigned_integral U>
auto less(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return less(lhs.digits(), rhs.digits()); }

template<std::unsigned_integral U>
auto greaterEqual(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return not less(lhs, rhs); }

template<std::unsigned_integral U>
auto greater(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return less(rhs, lhs); }

template<std::unsigned_integral U>
auto lessGreater(
    const BigUnsigned<U>& lhs,
    const BigUnsigned<U>& rhs
) -> bool { return not greater(lhs, rhs); }

}