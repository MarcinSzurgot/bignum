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

// template <class T1, class T2, class T3>
// std::ostream& operator<<(std::ostream& os, const std::tuple<T1, T2, T3>& tuple) {
//     os << "{" << std::get<0>(tuple) << ", " << std::get<1>(tuple) << ", " << std::get<2>(tuple) << "}";
//     return os;
// }