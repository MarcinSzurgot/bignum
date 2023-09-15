#include <bignum/BigUnsigned.hpp>

#include <iostream>

std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    os << std::hex << std::uppercase;
    for (auto d = 0u; d < num.mag(); ++d) {
        os << "0x" << num[d] << ", ";
    }
    return os;
}