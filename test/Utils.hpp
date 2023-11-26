#include <bignum/BigUnsigned.hpp>

#include <concepts>
#include <iostream>
#include <span>
#include <vector>

// inline std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
//     return os << static_cast<std::string>(num);
// }

inline std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    os << std::hex << "{0x" << num[0];
    for (auto d = 1u; d < num.mag(); ++d) {
        os << ", 0x" << num[d];
    }
    return os << "}";
}

template<std::unsigned_integral U>
inline std::ostream& operator<<(std::ostream& os, const std::vector<U>& num) {
    os << std::hex << "{0x" << num[0];
    for (auto d = 1u; d < size(num); ++d) {
        os << ", 0x" << num[d];
    }
    return os << "}";
}

template<std::unsigned_integral U>
inline std::ostream& operator<<(std::ostream& os, std::span<const U> num) {
    os << std::hex << "{0x" << num[0];
    for (auto d = 1u; d < size(num); ++d) {
        os << ", 0x" << num[d];
    }
    return os << "}";
}

struct BinaryOpParams {
    std::vector<std::uint32_t> lhs;
    std::vector<std::uint32_t> rhs;
    std::vector<std::uint32_t> expected;
};
