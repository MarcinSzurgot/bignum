#include <bignum/BigUnsigned.hpp>

#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const BigUnsigned& num) {
    return os << static_cast<std::string>(num);
}