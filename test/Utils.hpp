#include <bignum/BigUnsigned.hpp>

#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    // os << std::hex << std::uppercase << "{";
    // for (auto d = 1u; d < num.mag(); ++d) {
    //     os << "0x" << num[d - 1] << ", ";
    // }
    // os << "0x" << num[num.mag() - 1] << "}";
    return os << static_cast<std::string>(num);
}