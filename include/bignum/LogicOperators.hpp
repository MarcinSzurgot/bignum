#pragma once 

#include "BigUnsigned.hpp"

// namespace bignum {

// inline auto operator==(
//     const BigUnsigned& lhs,
//     const BigUnsigned& rhs
// ) -> bool { 
//     using namespace bignum;
//     return std::span(lhs.digits_) == std::span(rhs.digits_); 
// }

// inline auto operator!=(
//     const BigUnsigned& lhs,
//     const BigUnsigned& rhs
// ) -> bool { return !(lhs == rhs); }

// inline auto operator<(
//     const BigUnsigned& lhs,
//     const BigUnsigned& rhs
// ) -> bool {  
//     using namespace bignum;
//     return std::span(lhs.digits_) < std::span(rhs.digits_); 
// }

// inline auto operator>=(
//     const BigUnsigned& lhs,
//     const BigUnsigned& rhs
// ) -> bool { return !(lhs < rhs); }

// inline auto operator>(
//     const BigUnsigned& lhs,
//     const BigUnsigned& rhs
// ) -> bool { return rhs < lhs; }

// inline auto operator<=(
//     const BigUnsigned& lhs,
//     const BigUnsigned& rhs
// ) -> bool { return !(lhs > rhs); }

// }