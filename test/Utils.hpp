#include <bignum/BigUnsigned.hpp>

#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    os << std::hex << std::uppercase << "0x";
    for (auto d = 1u; d < num.mag(); ++d) {
        os << num[d - 1] << ", ";
    }
    os << num[num.mag() - 1];
    return os;
}