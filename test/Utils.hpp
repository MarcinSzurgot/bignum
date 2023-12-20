#include <bignum/BigUnsigned.hpp>

#include <concepts>
#include <iostream>
#include <span>
#include <vector>

using namespace bignum;

template<typename U>
requires std::unsigned_integral<std::remove_const_t<U>>
inline std::ostream& operator<<(std::ostream& os, std::span<const U> num) {
    os << std::hex << "{0x" << num[0];
    for (auto d = 1u; d < size(num); ++d) {
        os << ", 0x" << num[d];
    }
    return os << "}";
}

template<std::unsigned_integral U>
inline std::ostream& operator<<(std::ostream& os, const std::vector<U>& num) {
    return os << std::span(num);
}

inline std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    return os << num.digits<BigUnsigned::NativeDigit>();
}

template<
    typename LHS, 
    typename RHS, 
    typename Expected
>
struct BinaryOpParams {
    LHS lhs;
    RHS rhs;
    Expected expected;
};

using ArithmeticBinaryOp = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>
>;

using LogicBinaryOp = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::vector<std::uint32_t>,
    bool
>;

using ShiftOp = BinaryOpParams<
    std::vector<std::uint32_t>,
    std::size_t,
    std::vector<std::uint32_t>
>;