#pragma once 

#include "BigUnsigned.hpp"
#include "ArrayLogic/ArrayLogic.hpp"

namespace bignum {

inline auto operator==(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { 
    return lhs.digits<std::uint32_t>() == rhs.digits<std::uint32_t>(); 
}

inline auto operator!=(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool { return !(lhs == rhs); }

inline auto operator<(
    const BigUnsigned& lhs,
    const BigUnsigned& rhs
) -> bool {  
    return lhs.digits<std::uint32_t>() < rhs.digits<std::uint32_t>(); 
}

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