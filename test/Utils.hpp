#include <bignum/BigUnsigned.hpp>

#include <iostream>

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